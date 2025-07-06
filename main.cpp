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

            makefile.open("makefile");
            if(!makefile.is_open()){
                return -1;
            }

            return 0;
        }

        int fillFilesList(){
            #ifdef _WIN32
                system("dir /b /a:-d >> wmakeTemp.txt");
            #elif __LINUX__
                system("ls >> wmakeTempt.txt");
            #endif
            inputFile.open("wmakeTemp.txt");
            if (inputFile.is_open()){
                std::string tempStr;
                while(inputFile >> tempStr){
                    if (tempStr.find(sourceExt) != std::string::npos){
                        sourceFiles.push_back(tempStr);
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
            makefile << goal << ":" << std::endl << "\t" << compiler << " ";
            for (auto it = sourceFiles.begin(); it != sourceFiles.end(); ++it){
                makefile << *it << " "; 
            }
            makefile << "-o " << goal;

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
};

int main(int argc, char *argv[]){

    if (argc < 3){
        std::cout << "usage: wmake <compiler> <final exe name>\n";
        return 0;
    }

    fileList f;
    f.assignCompilerAndOpenFile(argv[1], argv[2]);
    f.fillFilesList();
    f.listContentsOfVector();
    f.writeMakeFile();
}