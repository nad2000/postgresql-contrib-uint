FROM postgres
COPY . /uint
WORKDIR /uint
RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get install -y --no-install-recommends gcc  postgresql-server-dev-all && \
    cd /uint && make  && make install && \
    apt-get remove -y gcc postgresql-server-dev-all && \
    apt-get autoremove -y  &&\
    apt-get autoclean -y  &&\
    cd /  && rm -rf /uint  &&\
    rm -rf /var/lib/apt/lists/* 
