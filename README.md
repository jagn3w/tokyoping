tokyoping
=========

My implementation of tokyo-ping. 

Tokyo-ping controls flow ids based on 5-tuple hashing to draw more
specific points along the latency distribution than ping, which
is a random sample of flow ids.

A lot more is required to use this as an enterprise-level latency estimator.

usage:
	./tokyoping [-i | -u] -t timeoutms -g gapms -r numprepeats -f numflows -h hostip
