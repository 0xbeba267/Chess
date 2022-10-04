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
	if (argc == 0) {
		cerr << "Error: Program connector need an argument.\n";
		exit(1);
	}

	// if program has run with argument "isready" 
	bool isready_test = (strcmp("isready", argv[1]) ? 0 : 1);

    int fd1[2], fd2[2];

	pid_t pid;
	if (pipe(fd1) == -1) {
		std::cerr << "Error while creating a pipe.\n";
		exit(1);
	}

	if (pipe(fd2) == -1) {
		std::cerr << "Error while creating a pipe.\n";
		exit(1);
	}

	switch (int pid = fork()) { /* Create a child process */
		case -1: {
			perror("fork for rev_child");
		}
		break;
		case 0: /* Child */
		{
			// save backup of stdin
			int default_input = dup(STDIN_FILENO);

			// replace stdin by input from first pipe
			dup2(fd1[INPUT_END], STDIN_FILENO);

			const char* cmd = "stockfish";

			FILE *pipe_fp;
			if (( pipe_fp = popen(cmd, "r")) == NULL)
			throw std::runtime_error("popen() failed!");

			std::array<char, 80> buffer;

			while (fgets(buffer.data(), buffer.size(), pipe_fp) != nullptr) {

					if (strncmp("readyok", buffer.data(), 7) == 0) {
						//(strncmp("readyok", buffer.data(), 8) == 0) 
						close(fd2[INPUT_END]);
						write(fd2[OUTPUT_END], &buffer, strlen(buffer.data()) );
						close(fd2[OUTPUT_END]);

						const char* polecenie_quit = "quit\n";

						// write 'quit' to first pipe (which is connected to stdin)
						write(fd1[OUTPUT_END], polecenie_quit, strlen(polecenie_quit));
						close(fd1[OUTPUT_END]);

						pclose(pipe_fp);

						break;
					}
				
				else 
				if (strncmp("bestmove", buffer.data(), 8) == 0) {
					// read output buffor continuously until not see 'bestmove'
					// write answer into second pipeline which goes to the parent
					close(fd2[INPUT_END]);
					write(fd2[OUTPUT_END], &buffer, strlen(buffer.data()) );
					close(fd2[OUTPUT_END]);

					const char* polecenie_quit = "quit\n";

					// write 'quit' to first pipe (which is connected to stdin)
					write(fd1[OUTPUT_END], polecenie_quit, strlen(polecenie_quit));
					close(fd1[OUTPUT_END]);

					pclose(pipe_fp);

					break;
				}
			}
			// restore stdin
			dup2(default_input, STDIN_FILENO);

			close(fd1[OUTPUT_END]);
			close(fd1[INPUT_END]);
			close(fd2[OUTPUT_END]);
			close(fd2[INPUT_END]);

			exit(0);				// end child process here
		} /* Child */
		break;
		default: /* Parent */
		{
			close(fd1[INPUT_END]);

			ostringstream input;

			// isready test detected - set input to "isready"
			if (isready_test) {
				input << "isready\n";
			}
			else
			{
			// send information about last move
				input << "position startpos moves ";
            	input << argv[1];
				input << "\ngo movetime 100\n";
			}
			

			// send data on through pipe to child process
			write(fd1[OUTPUT_END], input.str().c_str(), strlen( input.str().c_str()));

			wait(0);// run child process

			close(fd2[OUTPUT_END]);

			std::string output;
			char buf;

			// receive answer sent by child process through pipeline
			while (read(fd2[INPUT_END], &buf, 1) > 0) {
				output += buf;
			}

			close(fd1[INPUT_END]);
			close(fd1[OUTPUT_END]);
			close(fd2[OUTPUT_END]);
			close(fd2[INPUT_END]);

			// isready test detected - print output (which should be "readyok")
			if (isready_test) {
				cout << output;
			}
			else
			// TODO try decode 3 characters from second move to make promotion automatically
			// 3) Decode output to get the best move as indication
			{	
				string moveFromIndc = output.substr(9, 2);
				string moveToIndc = output.substr(11, 2);

				cout << "bestmove " << moveFromIndc << moveToIndc << endl;
			}
        } /* Parent */
    } /* switch (int pid = fork()) */

    return 0;
}