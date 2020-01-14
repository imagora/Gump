# -*- coding: utf-8 -*-
__author__ = "winking324@gmail.com"
__copyright__ = "Copyright (c) 2019 winking324"


import os
import datetime


class BaseConfig(object):
    DB_FILE = 'gump.sqlite'
    USER_REGEX = ''

    ONLINE_SCAN_INDEX_MEMBERS_URL = ''
    ONLINE_LIST_STREAMS_BY_KEY_URL = ''
    TRACER_URL = ''

    OAUTH_ID = ''
    OAUTH_KEY = ''
    OAUTH_PROXY = []

    SECRET_KEY = os.urandom(32)
    PERMANENT_SESSION_LIFETIME = datetime.timedelta(days=1)


