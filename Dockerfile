FROM nvidia/cuda:11.2.2-devel-ubuntu20.04 AS devel-base

ENV		NVIDIA_DRIVER_CAPABILITIES compute,utility,video
WORKDIR		/tmp/workdir

RUN	apt-get -yqq update && \
	apt-get install -yq --no-install-recommends build-essential sudo ca-certificates ssh expat libgomp1 && \
	apt-get autoremove -y && \
	apt-get clean -y

FROM nvidia/cuda:11.2.2-runtime-ubuntu20.04 AS runtime-base

ENV		NVIDIA_DRIVER_CAPABILITIES compute,utility,video
WORKDIR		/tmp/workdir

RUN	apt-get -yqq update && \
	apt-get install -yq --no-install-recommends build-essential sudo ca-certificates ssh expat libgomp1 libxcb-shape0-dev && \
	apt-get autoremove -y && \
	apt-get clean -y

FROM devel-base as build

ENV	DEBIAN_FRONTEND="noninteractive" \
	TZ="America/Los_Angeles"

RUN	buildDeps="autoconf \
		   automake \
		   curl \
		   ffmpeg \
		   g++ \
		   gcc \
		   git \
		   libssl-dev \
		   make \
		   openssl \
		   pkg-config \
		   yasm" && \
	apt-get -yqq update && \
	apt-get install -yq --no-install-recommends ${buildDeps}
	
RUN	apt-get -yqq update && \	
	apt-get install -y libsdl2-dev libvdpau-dev libva-dev libxcb-shm0-dev libpostproc-dev libgl1-mesa-dev libsdl2-dev freeglut3 freeglut3-dev
	
RUN	apt-get update -y && \
	apt-get install -y iputils-ping
	
RUN	apt-get update -y  && \
	apt-get install -y net-tools
	
RUN	apt-get update -y && \
	sudo apt install -y nano



COPY	. /OpenUVR
WORKDIR /OpenUVR/sending


## nv-codec
RUN \
	DIR=/tmp/nv-codec-headers && \
	mkdir -p ${DIR} && \
	cd ${DIR} && \
	git clone https://git.videolan.org/git/ffmpeg/nv-codec-headers.git ${DIR} && \
	make && \
	make install && \
	rm -rf ${DIR}

RUN	codecs="libass-dev \
		libfdk-aac-dev \
		libgles2-mesa-dev \
		libmp3lame-dev \
		libpulse-dev \
		libvorbis-dev \
		libx264-dev \
		libx265-dev \
		libtheora-dev" && \
	apt-get install -yq --no-install-recommends ${codecs}

RUN	make ffmpeg 

RUN	make

RUN	mkdir /usr/local/include/openuvr/

RUN	make install
	
WORKDIR /OpenUVR/quake

RUN	apt-get -yqq update && \
	apt install -y libxcb-shape0-dev libxcb-xfixes0-dev
	
RUN	./make_quake.sh

RUN	cp /OpenUVR/cd/* /home/root/bin/ioquake3/baseq3/

RUN	mkdir /home/root/bin/ioquake3/mymod/
RUN 	mkdir /home/root/bin/ioquake3/mymod/vm/
	
RUN 	cp /OpenUVR/quake/ioq3/build/release-linux-x86_64/baseq3/vm/* /home/root/bin/ioquake3/mymod/vm/

#CMD	["./run_quake.sh"]

