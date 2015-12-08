#!/bin/bash

awk 'NR<=3333333' pr_data > pr_data1
awk 'NR>3333333&&NR<=6666666' pr_data > pr_data2
awk 'NR>6666666' pr_data > pr_data3


