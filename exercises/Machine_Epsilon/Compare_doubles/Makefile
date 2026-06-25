Out.txt : cdou             # Out.txt depends on hello.exe
	./cdou > Out.txt        # run hello.exe, send output to Out.txt
cdou: main.cc                 # hello depends on hello.cc
	c++ -o cdou main.cc  # compile-and-link hello.cc, save executable in hello
clean:
	rm -f Out.txt cdou
