FROM --platform=linux/amd64 opensuse/tumbleweed
LABEL maintainer.name="The Xen Project" \
      maintainer.email="xen-devel@lists.xenproject.org"

ENV USER root

RUN mkdir /build
WORKDIR /build

RUN zypper ref && zypper dup -y --no-recommends
RUN zypper install -y --no-recommends \
        acpica \
        bc \
        bison \
        bzip2 \
        checkpolicy \
        clang \
        cmake \
        diffutils \
        discount \
        flex \
        gawk \
        gcc \
        gcc-c++ \
        git \
        ghostscript \
        glib2-devel \
        glibc-devel \
        gzip \
        hostname \
        libaio-devel \
        libbz2-devel \
        libext2fs-devel \
        libgnutls-devel \
        libjpeg62-devel \
        libnl3-devel \
        libnuma-devel \
        libpixman-1-0-devel \
        libpng16-devel \
        libssh2-devel \
        libtasn1-devel \
        libuuid-devel \
        libyajl-devel \
        libzstd-devel \
        lzo-devel \
        make \
        meson \
        nasm \
        ncurses-devel \
        ninja \
        ocaml \
        ocaml-findlib-devel \
        ocaml-ocamlbuild \
        ocaml-ocamldoc \
        pandoc \
        patch \
        pkg-config \
        'pkgconfig(libpci)' \
        'pkgconfig(sdl)' \
        'pkgconfig(sdl2)' \
        python3-devel \
        python3-setuptools \
        # systemd-devel for Xen < 4.19
        systemd-devel \
        tar \
        transfig \
        valgrind-devel \
        wget \
        which \
        xz-devel \
        zlib-devel \
        && \
        zypper clean -a
