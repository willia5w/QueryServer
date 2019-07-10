  # Created by Dan Williams, 7/10/2019
  
  #!/bin/bash
  
  echo "cores: $(cat /proc/cpuinfo | grep processor | wc -l)"
  
  echo "memory: free $(awk '/MemFree/ {printf("%.2f", $2 / 1024)}' /proc/meminfo) MB / total $(awk '/MemTotal/ {printf("%.2f\n", $2 / 1024)}' /proc/meminfo) MB"
  
  echo "primary partition storage: free $(df -hl --total | grep total | awk {'print $4'})B / total $(df -hl --total | grep total | awk {'print $2'})B "
