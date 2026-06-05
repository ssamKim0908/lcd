echo 1 > /tmp/stress_n
sudo bpftrace client_key.bt $(cat /tmp/stress_n) > test.csv