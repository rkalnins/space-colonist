rundbg:
	./cmake-build-debug/sc
	reset

test:
	./cmake-build-debug/sc_test < config.json -d yes


cleanlogs:
	rm -f logs/*
