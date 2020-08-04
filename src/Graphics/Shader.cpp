#include "Shader.hpp"

enum ReadingMode {
    VERTEX,
    FRAGMENT,
    ATTRIBUTES,
    UNIFORMS
};

void Shader::ReadFromFile(const char* ShaderFile) {

    ifstream File;
    
    if (!FileManager::GetFile(ShaderFile, File)) {
        Debug::Alert("Shader File - " + (string)ShaderFile + " Not Found");
        return;
    }

    Path = ShaderFile;

    string VertexShader = "";
    string FragmentShader = "";

    ReadingMode readingMode = VERTEX; 
    bool ReadingChange = false;
    
    string Line;
    while (getline(File, Line)) {

        if (Line[0] == '#') {
            if (Line == "#VertexShader") {
                readingMode = VERTEX;
                ReadingChange = true;
            }
            else if (Line == "#FragmentShader") {
                readingMode = FRAGMENT;
                ReadingChange = true;
            }
            else if (Line == "#Attributes") {
                readingMode = ATTRIBUTES;
                ReadingChange = true;
            }
            else if (Line == "#Uniforms") {
                readingMode = UNIFORMS;
                ReadingChange = true;
            }
        }

        if (ReadingChange)
            ReadingChange = false;
        else {
            if (readingMode == VERTEX)
                VertexShader += Line + "\n";
            else if (readingMode == FRAGMENT)
                FragmentShader += Line + "\n";
            else if (readingMode == ATTRIBUTES) {
                if (is_number(Line))
                    Attributes.push_back(stoi(Line));
            }
            else if (readingMode == UNIFORMS)
                if (Line != "")
                    Uniforms.push_back(Uniform(Line));
        }
    }

    char* Error = new char[512];

    if (!CompileShader(VertexShader.c_str(), VertexId, GL_VERTEX_SHADER, Error))
        Debug::Alert("Faild To Compile Vertex Shader With Error: \n" + (string)Error);

    if (!CompileShader(FragmentShader.c_str(), FragmentId, GL_FRAGMENT_SHADER, Error))
        Debug::Alert("Faild To Compile Fragment Shader With Error: \n" + (string)Error);

    if (!CompileProgram(ProgramId, vector<unsigned int>({VertexId, FragmentId}), Error))
        Debug::Alert("Faild To Link Shader Program With Error: \n" + (string)Error);

    for (long unsigned int Index = 0; Index < Uniforms.size(); Index++)
        Uniforms[Index].Id = glGetUniformLocation(ProgramId, Uniforms[Index].Name.c_str());

    glDeleteShader(VertexId);
    glDeleteShader(FragmentId);
}

bool Shader::CompileShader(const char* ShaderDta, unsigned int& Id, unsigned int ShaderType, char* Error) {

    Id = glCreateShader(ShaderType);
    glShaderSource(Id, 1, &ShaderDta, NULL);
    glCompileShader(Id);

    return CheckShader(Id, Error);
}

bool Shader::CompileProgram(unsigned int& Id, vector<unsigned int> Shaders, char* Error) {

    Id = glCreateProgram();
    
    int Size = Shaders.size();
    
    for (int Index = 0; Index < Size; Index++)
        glAttachShader(Id, Shaders[Index]);

    glLinkProgram(Id);

    return CheckProgram(ProgramId, Error);
}

bool Shader::CheckShader(unsigned int ShaderId, char* Error) {

    int success;
    glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &success);

    if (success)
        return true;

    glGetShaderInfoLog(ShaderId, 512, NULL, Error);

    return false;
}

bool Shader::CheckProgram(unsigned int ProgramId, char* Error) {

    int success;
    glGetProgramiv(ProgramId, GL_LINK_STATUS, &success);
    
    if (success)
        return true;

    glGetProgramInfoLog(ProgramId, 512, NULL, Error);
    
    return false;
}

void Shader::Bind() {
    glUseProgram(ProgramId);
}

void Shader::UnBind() {
    glUseProgram(0);
}

unsigned int Shader::GetUniformId(string Name) {

    size_t Size = Uniforms.size();
    
    for (size_t Index = 0; Index < Size; Index++)
        if (Uniforms[Index].Name == Name)
            return Uniforms[Index].Id;

    return 0;
}