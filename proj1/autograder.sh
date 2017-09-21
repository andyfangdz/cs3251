#!/usr/bin/env bash

PORT=5432
HOST=127.0.0.1

./server & PID_SERVER=$!
./client & PID_CLIENT=$!
