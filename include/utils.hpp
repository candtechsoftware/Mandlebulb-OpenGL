#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

#ifdef __APPLE__
#    include <OpenGL/gl3.h>
#    include <OpenGL/glext.h>
#else
#    include <GL/gl.h>
#    include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

namespace utils {
unsigned char* read_file(char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == nullptr) {
        std::cout << "Error: Cannot open file" << filename << std::endl;
    }

    long bytes;
    long saved_pos = ftell(file);
    fseek(file, 0, SEEK_END);
    bytes = ftell(file);
    fseek(file, saved_pos, SEEK_SET);
    long bytes_in_file = bytes;

    auto* buffer = (unsigned char*)malloc(bytes_in_file + 1);
    auto bytes_read = (int)fread(buffer, 1, bytes_in_file, file);
    buffer[bytes_read] = 0; // terminate string with 0
    fclose(file);

    return buffer;
}

void print_shader_info(GLuint obj, const char* fn)
{
    GLint info_log_len = 0;
    GLint chars_written = 0;
    char* log_info;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &info_log_len);
}
void printProgramInfoLog(
    GLuint obj, const char* vfn, const char* ffn, const char* gfn, const char* tcfn, const char* tefn)
{
    GLint infologLength = 0;
    GLint charsWritten = 0;
    char* infoLog;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 2) {
        if (gfn == nullptr)
            fprintf(stderr, "[From %s+%s:]\n", vfn, ffn);
        else if (tcfn == nullptr || tefn == nullptr)
            fprintf(stderr, "[From %s+%s+%s:]\n", vfn, ffn, gfn);
        else
            fprintf(stderr, "[From %s+%s+%s+%s+%s:]\n", vfn, ffn, gfn, tcfn, tefn);
        infoLog = (char*)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        fprintf(stderr, "%s\n", infoLog);
        free(infoLog);
    }
}

GLuint compileShaders(
    const unsigned char* vertAssembly, const unsigned char* fragAssembly, const char* vertName, const char* fragName)
{
    GLuint vertShader, fragShader, pID;
    const char *vertShaderStrings[1], *fragShaderStrings[1];
    int vertSuccess, fragSuccess, shadersLinked;
    char infoLog[4096];

    vertShader = glCreateShader(GL_VERTEX_SHADER);
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    vertShaderStrings[0] = (char*)vertAssembly;
    fragShaderStrings[0] = (char*)fragAssembly;

    glShaderSource(vertShader, 1, vertShaderStrings, nullptr);
    glShaderSource(fragShader, 1, fragShaderStrings, nullptr);
    glCompileShader(vertShader);
    glCompileShader(fragShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &vertSuccess);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragSuccess);

    if (!vertSuccess) {
        glGetShaderInfoLog(vertShader, sizeof(infoLog), nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    if (!fragSuccess) {
        glGetShaderInfoLog(fragShader, sizeof(infoLog), nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    pID = glCreateProgram();
    glAttachShader(pID, vertShader);
    glAttachShader(pID, fragShader);
    glBindAttribLocation(pID, 0, "in_Position");
    glLinkProgram(pID);
    glGetProgramiv(pID, GL_LINK_STATUS, &shadersLinked);

    if (shadersLinked == GL_FALSE) {
        glGetProgramInfoLog(pID, sizeof(infoLog), nullptr, infoLog);
        std::cout << "Program object linking error'n" << infoLog << std::endl;
    }

    // glUseProgram(p);
    glDetachShader(pID, vertShader);
    glDetachShader(pID, fragShader);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    printShaderInfoLog(vertShader, vertName);
    printShaderInfoLog(fragShader, fragName);
    return pID;
}

GLuint loadShaders(const char* vertFileName, const char* fragFileName)
{
    unsigned char *vs, *fs;
    GLuint program = 0;

    vs = readFile((char*)vertFileName);
    fs = readFile((char*)fragFileName);

    if (vs == nullptr) {
        perror("Failed to read vertex from disk.\n");
    }
    if (fs == nullptr)
        fprintf(stderr, "Failed to read %s from disk.\n", fragFileName);

    if ((vs != nullptr) && (fs != nullptr)) {
        program = compileShaders(vs, fs, vertFileName, fragFileName);
    }

    if (vs != nullptr)
        free(vs);
    if (fs != nullptr)
        free(fs);
    return program;
}

void showUsage()
{
    std::cerr << "Usage: ./mandelbulb -c\n"
              << "Options:\n"
              << "\t-h,--help\t\tShow this message\n"
              << "\t-w,--weak \t\tLower settings for weak computer i.e. shitty Intel HD graphics laptop\n"
              << "\t-c,--coordinates \tLog coordinates in console every frame \n\n"
              << "Controls:\n"
              << "\tQ \tQuit the program\n"
              << "\tL \tReload shaders\n"
              << "\tWASD \tMovement around center\n"
              << "\tZ \tZoom in\n"
              << "\tX \tZoom out\n"
              << "\tR \tReset position\n"
              << std::endl;
}

void printInstructions()
{
    std::cout << "Keys:\n"
              << "Q: Quit\n"
              << "L: Reload shader files\n"
              << "WASD: Movement around center\n"
              << "Z: Zoom out\n"
              << "X: Zoom in\n"
              << "R: Reset position\n"
              << "G: Show/hide GUI\n";
}

int handleArgs(int c, char* argv[], bool& logCoordinates, bool& weakSettings)
{
    for (int i = 1; i < c; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            showUsage();
            return -1;
        } else if (arg == "-w" || arg == "--weak") {
            weakSettings = true;
        } else if (arg == "-c" || arg == "--coordinates") {
            logCoordinates = true;
        }
    }
    return 0;
}

} // namespace utils