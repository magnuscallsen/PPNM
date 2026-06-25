Out.txt : non_com             # Out.txt depends on hello.exe
	./non_com > Out.txt        # run hello.exe, send output to Out.txt
non_com: main.cc                 # hello depends on hello.cc
	c++ -o non_com main.cc  # compile-and-link hello.cc, save executable in hello
clean:
	rm -f Out.txt non_com
