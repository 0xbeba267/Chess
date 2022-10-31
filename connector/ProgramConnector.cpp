/*	Program's goal is to run external chess engine with commands,
 *  receiving and print result then close chess engine.
 *  Pipe is used to send arguments into input of forked process.

 *  1) Parent process takes two arguments:
 	 argv[1] - command for chess engine ex. stockfish
	 argv[2] - commands to call by chess engine ex. "isready\n"
    and writes them into a pipe.
 *  2) Child process runs chess engine with replaced stdid from pipe,
 *  then it writes the line with result on stdout and closes chess engine.
 * 
 *  Line with result is recognized by keyword "bestmove" or
 *  "readyok" if program was called with "isready\n" (for cheking if connection works correctly)
 */ 

#include <iostream>
#include <string.h>
#include <sstream>
#include <array>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

#define INPUT_END 0		// INPUT_END means where the pipe takes input
#define OUTPUT_END 1	// OUTPUT_END means where the pipe produces output

int main(int argc, char* argv[]) {
	if (argc <= 2) {
		cerr << "Error: Program connector needs two arguments - with engine name and command to send.\n";
		exit(1);
	}

	const char* keyword = (strcmp("isready\n", argv[2]) ? "bestmove" : "readyok");

	// create pipe
    int fd[2];
	if (pipe(fd) == -1) {
		std::cerr << "Error while creating a pipe.\n";
		exit(1);
	}

	pid_t pid;
	/* Program splits here,
	*  child starts after calling wait(0). */
	switch (int pid = fork()) { 
		case -1: {
			perror("fork for rev_child");
		}
		break;
		case 0: /* Child */
		{
			// save descriptor to stdin
			int default_input = dup(STDIN_FILENO);

			// replace stdin by input from the pipe
			dup2(fd[INPUT_END], STDIN_FILENO);

			// get a name of chess engine to run
			const char* cmd = argv[1];

			// open chess engine using popen to read output
			FILE *pipe_fp;
			if (( pipe_fp = popen(cmd, "r")) == NULL)
			throw std::runtime_error("popen() failed!");

			std::array<char, 80> buffer;

			while (fgets(buffer.data(), buffer.size(), pipe_fp) != nullptr) {
			// read output buffor continuously until not see keyword
				if (strncmp(keyword, buffer.data(), strlen(keyword)) == 0) {
				
					cout << buffer.data();

					const char* polecenie_quit = "quit\n";

					// write 'quit' to pipe (satdin) to close process
					write(fd[OUTPUT_END], polecenie_quit, strlen(polecenie_quit));
					close(fd[OUTPUT_END]);

					break;
				}
			}
			pclose(pipe_fp);

			// restore stdin
			dup2(default_input, STDIN_FILENO);

			
			close(fd[INPUT_END]);
			close(fd[OUTPUT_END]);

			exit(0);				// end child process here
		} /* Child */
		break;
		default: /* Parent */
		{
			ostringstream input;

			input << argv[2];

			// send data through pipe to child process
			write(fd[OUTPUT_END], input.str().c_str(), strlen( input.str().c_str()));
			

			wait(0); // run child process
			
			close(fd[OUTPUT_END]);
			close(fd[INPUT_END]);
        } /* Parent */
    } /* switch (int pid = fork()) */

    return 0;
}