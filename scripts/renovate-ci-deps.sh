#!/bin/sh
#
# kas - setup tool for bitbake based projects
#
# Copyright (c) Siemens AG, 2026
#
# Authors:
#  Jan Kiszka <jan.kiszka@siemens.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

for file in $(git grep -l " uses: [^\.]" .github); do
	git grep -h " uses: [^\.]" "$file" | \
	    sed 's/.* uses: //g' | sort | uniq | \
	    while read -r cur_entry; do
		url=${cur_entry%%@*}
		latest=$(git ls-remote --refs --tags "https://github.com/$url" | sort -r -V -k 2 | head -1)
		tag=${latest##*refs/tags/}
		new_entry="$url@${latest%%"$(printf '\t')"*}  # $tag"
		if [ "$new_entry" != "$cur_entry" ]; then
			echo "Updating $file to '$new_entry'"
			released=$(curl -s "https://api.github.com/repos/$url/releases/tags/$tag" | jq -r '.published_at')
			echo "(published ${released%%T*})"
			sed -i "s|$cur_entry|$new_entry|g" "$file"
		fi
	done
done
