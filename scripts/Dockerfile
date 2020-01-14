FROM ubuntu:18.04

ENV DEBIAN_FRONTEND noninteractive

RUN sed -i "s/archive.ubuntu.com/mirrors.aliyun.com/g" /etc/apt/sources.list && \
    sed -i "s/security.ubuntu.com/mirrors.aliyun.com/g" /etc/apt/sources.list && \
    apt-get update && \
    apt-get install -y --no-install-recommends \
        python3-pip \
        python3-setuptools \
        sqlite3 \
        uwsgi \
        uwsgi-plugin-python3 && \
    rm -rf /var/lib/apt/lists/*

RUN pip3 install -i https://pypi.tuna.tsinghua.edu.cn/simple \
    flask \
    flask-sessionstore \
    requests \
    wheel

ADD gump_server /data/app/gump_server

WORKDIR /data/app/gump_server
RUN mkdir -p uwsgi log

RUN LC_ALL=C.UTF-8 LANG=C.UTF-8 FLASK_APP=app FLASK_ENV=production flask init-db

CMD ["/data/app/gump_server/setup.sh"]

