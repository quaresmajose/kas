The ``kas-container`` script is a wrapper to run `kas` inside a build
container. It gives fine grained control over the data that is mapped into the
build and decouples the build environment from the host system. For details,
see :ref:`env-vars-label`. The wrapper also takes care of mounting the
necessary directories and setting up the environment variables inside the
container.

.. note::
    The ``kas-container`` script has limited support for Git worktrees. Regular
    Git operations on the checked-out repository are supported. However,
    executing any ``git worktree ...`` command inside the container is not
    allowed.

As container backends, Docker and Podman are supported. If both backends are
installed, Docker is preferred, except when the build system is
``isar-rootless``, in which case Podman is the default. To force the use of a
specific backend, set, e.g., ``KAS_CONTAINER_ENGINE=podman``. See
:ref:`env-vars-label` for details.

The simplest and therefore recommended setup for secure rootless operation is
via Podman. For building in ``isar-rootless`` mode, the host kernel version
must be v6.7 or newer.

Running under docker in
`rootless mode <https://docs.docker.com/engine/security/rootless/>`_ is
partially supported. For rootless Docker, it is recommended to use a distinct
``KAS_WORK_DIR`` outside of the calling directory (repo-dir), as kas
temporarily changes the ownership of the working directory during its
operation. All files managed by kas (including the repos) must not be written
to from the host. To completely remove all data managed by kas, use
``kas-container purge``. This also restores the directory owners of the dirs
passed to kas, so they can be removed from the host.

If you need to use Docker in system mode instead, make sure that the invoking
user is part of the respective group. System Podman will be invoked via
``sudo`` by ``kas-container``, and you will have to configure the system
accordingly so that this path is permitted. However, be warned about the
security implications of both of these setups as they are granting root
permissions to the build jobs.

.. note::
    The ISAR build system is compatible with rootless execution in
    ``isar-rootless`` mode only. The ``isar`` and ``isar-privileged`` modes
    fall back to the system Docker or Podman instance.

By default ``kas-container`` uses the official images provided by the kas
project: ``ghcr.io/siemens/kas/kas[-isar]:<version>``. To specify your own
image set the ``KAS_CONTAINER_IMAGE`` environment variable. The
``kas-container`` script version should match the kas version inside the
container. If kas detects that is was called from ``kas-container`` and the
versions do not match, a warning is emitted. This limitation might be lessened
in the future, once a stable interface between ``kas-container`` and kas is
introduced.

From version ``5.0`` onward, kas offers images built on several base
distributions. Select a distribution by setting the environment variable
``KAS_CONTAINER_IMAGE_DISTRO`` to the desired value (e.g. ``debian-bookworm``
or ``debian-trixie``). The corresponding image tags follow the pattern
``:<version>-<base-distro>`` (e.g. ``:5.0-debian-bookworm``).
Alternatively, you can adjust ``KAS_CONTAINER_IMAGE_DISTRO_DEFAULT`` in the
``kas-container`` script if you copy this into your downstream layer already
for encoding the supported kas version.
