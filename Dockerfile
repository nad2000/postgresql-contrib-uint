FROM postgres:9
COPY . /uint
RUN apt-get update -y && apt-get upgrade -y \
    && apt-get install -y --no-install-recommends \
      gcc  \
      build-essential \
      postgresql-server-dev-all \
    && cd /uint && PATH=/usr/lib/postgresql/${PG_MAJOR}/bin:$PATH make install \
    && apt-get purge -y build-essential binutils comerr-dev cpp dctrl-tools gcc krb5-multidev libasan* libatomic1 libc-dev-bin libc6-dev libcilkrts5 libcloog-isl4 libgomp1 libgssrpc4 libisl* libitm1 libkadm5clnt-mit9 libkadm5srv-mit9 libkdb5-7 liblsan0 libmpc3 libmpfr4 libpq-dev libquadmath0 libssl-dev libtsan0 libubsan0 linux-libc-dev make postgresql-server-dev-all zlib1g-dev \
    && apt-get autoremove -y \
    && apt-get autoclean -y \
    && cd / && rm -rf /uint \
    && rm -rf /var/lib/apt/lists/*
