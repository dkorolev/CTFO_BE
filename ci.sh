#!/bin/bash

rm -rf CI
mkdir CI
cd CI

mkdir -p CTFO_BE/crunchers
(cd ..; cp -r *.cc *.h crunchers golden CI/CTFO_BE ; tail -n +7 cards.txt > CI/CTFO_BE/cards.txt)

git clone https://github.com/C5T/Current

(cd CTFO_BE ; ln -sf ../Current/scripts/Makefile .)
(cd CTFO_BE ; make .current/prepare_cards ; ./.current/prepare_cards)

(cd CTFO_BE ; make test)
