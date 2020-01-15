# -*- coding: utf-8 -*-
__author__ = "winking324@gmail.com"
__copyright__ = "Copyright (c) 2019 winking324"


import db
import re
import time


RULES = {}
LAST_LOAD_TS = 0


def load_rules():
    database = db.get_db()
    rules = {}
    for key, regex, url in database.execute('SELECT * FROM rules'):
        rules[key] = {'re': regex, 'url': url}
    return rules


def convert_to_play_stream(logger, url):
    global RULES
    global LAST_LOAD_TS

    now = int(time.time())
    if now - LAST_LOAD_TS > 60:
        RULES = load_rules()
        LAST_LOAD_TS = now

    if not RULES:
        logger.warn('convert to play stream failed for empty rules')
        return None

    rule = None
    for k, v in RULES.items():
        if re.search(k, url) is None:
            continue

        rule = v
        break

    if rule is None:
        logger.warn('convert to play stream failed for not found rule')
        return None

    try:
        info = re.findall(rule['re'], url).pop()
        stream = rule['url'] % (tuple(info) if isinstance(info, list) else info)
        logger.info('convert url {} to play stream {}'.format(url, stream))
    except Exception as e:
        logger.warn('convert url {} failed: {}'.format(url, repr(e)))
        stream = None
    return stream

