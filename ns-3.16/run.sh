#!/bin/bash
for i in `seq 3`
do
	./waf --run scratch/topology-example-sim > log$i &
done
