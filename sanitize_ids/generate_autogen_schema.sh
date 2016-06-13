#!/bin/bash

(curl -s "http://localhost:8383/raw_log?schema=h" > .current_autogen_schema.h \
 && mv .current_autogen_schema.h autogen_schema.h \
 && echo 'OK') || echo 'The CTFO server must be running to regenerate `autogen_schema.h`.'
