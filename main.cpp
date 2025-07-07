#include <iostream>
#include <fstream>
#include <vector>

class fileList{
    private:
        std::string compiler;
        std::string goal;
        std::vector<std::string> sourceFiles, headerFiles;
        std::vector<std::pair<std::string, std::string>> filePairs;
        std::ifstream inputFile;
        std::ofstream makefile;
        std::string headerExt, sourceExt;
    public:
        int assignCompilerAndOpenFile(std::string compiler, std::string goal){
            this->goal = goal;
            this->compiler = compiler;
            if(compiler == "g++"){
                headerExt = ".hpp";
                sourceExt = ".cpp";
            }
            else if(compiler == "gcc"){
                headerExt = ".h";
                sourceExt = ".c";
            }
            else{
                std::cout << "not C/C++ compiler" << std::endl;
                return -1;
            }

            return 0;
        }

        int fillFilesList(){
            /*#ifdef _WIN32
                system("dir /b /a:-d >> wmakeTemp.txt");
            #else*/
                system("ls >> wmakeTemp.txt");
            //#endif
            inputFile.open("wmakeTemp.txt");
            if (inputFile.is_open()){
                std::string tempStr = "";
                while(inputFile >> tempStr){
                    if (tempStr.find(sourceExt) != std::string::npos){
                        sourceFiles.push_back(tempStr);
                        filePairs.push_back(std::make_pair(tempStr.substr(0, tempStr.find_first_of('.', 0)) + sourceExt, tempStr.substr(0, tempStr.find_first_of('.', 0)) + ".o"));
                    }
                    else if(tempStr.find(headerExt) != std::string::npos){
                        headerFiles.push_back(tempStr);
                    }  
                }
                
                inputFile.close();
            }

            std::remove("wmakeTemp.txt");
            return 0;
        }

        void writeMakeFile(){
            makefile.open("makefile");
            if(!makefile.is_open()){
                return;
            }
            makefile << goal << ":";
            for (auto it = filePairs.begin(); it != filePairs.end(); ++it){
                makefile << " " << it->second; 
            }
            makefile << "\n\t" << compiler;
            for (auto it = filePairs.begin(); it != filePairs.end(); ++it){
                makefile << " " << it->second; 
            }
            makefile << " -o " << goal << "\n";

            for (auto it = filePairs.begin(); it != filePairs.end(); ++it){
                makefile << it->second << ": " << it->first << "\n\t";
                makefile << compiler << " -c " <<  it->first << " -o " << it->second << "\n";
            }
		/*
            #ifdef _WIN32
                makefile << std::endl << "clean:\n\tdel /Q";
                for (auto it = filePairs.begin(); it != filePairs.end(); ++it){
                    makefile << " " << it->second;  
                }
                makefile << " " << goal + ".exe";

                makefile << std::endl << "c:\n\tdel /Q";
                for (auto it = filePairs.begin(); it != filePairs.end(); ++it){
                    makefile << " " << it->second;  
                }
            #else*/
                makefile << std::endl << "clean:\n\trm -f";
                for (auto it = filePairs.begin(); it != filePairs.end(); ++it){
                    makefile << " " << it->second;  
                }
                makefile << " " << goal;

                makefile << std::endl << "c:\n\trm -f";
                for (auto it = filePairs.begin(); it != filePairs.end(); ++it){
                    makefile << " " << it->second;  
                }
            //#endif


        }

        void listContentsOfVector(){
            int hNumber = 1;
            int sNumber = 1;
            std::cout << "source files:\n";
            for (auto it = sourceFiles.begin(); it != sourceFiles.end(); ++it){
                std::cout << sNumber << "\t" << *it << '\n'; 
                sNumber++;
            }

            std::cout << "header files:\n";
            for (auto it = headerFiles.begin(); it != headerFiles.end(); ++it){
                std::cout << hNumber << "'\t" << *it << '\n'; 
                hNumber++;
            }
        }

        int readFromExistingmakefile(){
            std::ifstream file("makefile");
            if (file.is_open()){
                std::string target; 
                std::string tempCompiler;
                file >> target;
                file >> tempCompiler;
                goal = target.substr(0, target.find_first_of(':', 0));
                compiler = tempCompiler;
                return 0;
            }
            else {
                std::cout << "no existing makefile\n";
                return -1;
            }
        }
};

int main(int argc, char *argv[]){
    fileList f;
    if (argc < 3){
        std::cout << "usage: wmake <compiler> <final exe name>\n";
        return 0;
    }

    f.assignCompilerAndOpenFile(argv[1], argv[2]);
    f.fillFilesList();
    f.listContentsOfVector();
    f.writeMakeFile();
}
