.PHONY: check demo clean
check:
	./scripts/check_all.sh
demo:
	./engine-cpp/build/resurge-engine
	PYTHONPATH=lab-python python3 -m resurge_lab.cli benchmark --rate .03 --seed 17 --cluster 8
clean:
	rm -rf engine-cpp/build studio-ts/dist plugin-java/out
