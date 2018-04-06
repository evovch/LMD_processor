#!/bin/bash

cd ../build-LMD_processor-Desktop-Debug/
rootcling -f eventsdict.cxx -c ../LMD_processor/data/cls_RootHit.h ../LMD_processor/data/cls_RootEvent.h
cd -
