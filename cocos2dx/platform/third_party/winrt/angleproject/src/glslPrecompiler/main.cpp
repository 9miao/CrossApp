//
// Copyright (c) 2002-2013 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

#include "../samples/gles2_book/Common/esUtil.h"
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <string>

static void usage();
static GLuint createShader(GLenum shaderType, const char **source, unsigned sourceCount);

int main(int argc, char* argv[])
{
    ESContext esContext;
    esInitContext(&esContext);
    esCreateWindow(&esContext, TEXT("GLSL Precompiler"), 1, 1, ES_WINDOW_RGB);

    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
    bool usageFail = false;
    bool outputToHeaderFile = false;
    char outputFile[1024] = "shader.file";
    char variableName[1024];
    std::vector<std::string> vertexShaders;
    std::vector<std::string> fragmentShaders;

    if (argc < 3)
        usageFail = TRUE;
    
    GLuint program = glCreateProgram();

    argc--;
    argv++;
    for (; (argc >= 1) && !usageFail; argc--, argv++) {
        if (argv[0][0] == '-') {
            switch (argv[0][1]) {
            case 'o':
                if (argv[0][2] == '=' && strlen(argv[0] + 3)) {
                    strcpy(outputFile, argv[0] + 3);
                }
                break;
            case 'a':
                if (argv[0][2] == '=') {
                    strcpy(variableName, argv[0] + 3);
                    outputToHeaderFile = true;
                }
                break;
            case 'v':
            case 'f':
                {
                    if(argv[0][2] != '=')
                        break;

                    FILE *fp = fopen(argv[0] + 3, "r");
                    if(!fp)
                    {
                        printf("Can't open %s, aborting\n", argv[0]);
                        return 0;
                    }
                    fseek(fp, 0, SEEK_END);
                    int fileSize = ftell(fp);
                    rewind(fp);
                    char *sourceBuffer = new char[fileSize + 1];
                    fileSize = fread(sourceBuffer, 1, fileSize, fp);
                    fclose(fp);
                    sourceBuffer[fileSize] = 0;
                    if(argv[0][1] == 'v')
                        vertexShaders.push_back(sourceBuffer);
                    else
                        fragmentShaders.push_back(sourceBuffer);
                    delete [] sourceBuffer;
                }
                break;
            default: usageFail = true;
            }
        }
    }

    if (usageFail)
    {
        usage();
        return -1;
    }

    std::vector<const char*> vertexSources;
    std::vector<const char*> fragmentSources;
    for(unsigned i = 0; i < vertexShaders.size(); ++i)
        vertexSources.push_back(vertexShaders[i].c_str());
    for(unsigned i = 0; i < fragmentShaders.size(); ++i)
        fragmentSources.push_back(fragmentShaders[i].c_str());

    vertexShader = createShader(GL_VERTEX_SHADER, &vertexSources[0], vertexSources.size());
    if(!vertexShader)
        return 0;
    fragmentShader = createShader(GL_FRAGMENT_SHADER, &fragmentSources[0], vertexSources.size());
    if(!fragmentShader)
        return 0;
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    int linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if(linkStatus != GL_TRUE)
    {
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &linkStatus);
        char *infoLog = new char[linkStatus];
        glGetProgramInfoLog(program, linkStatus, NULL, infoLog);
        printf("Failed to link the shaders into a program due to:\n%s\n", infoLog);
        delete [] infoLog;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }
    glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH_OES, &linkStatus);




    unsigned char *binary = new unsigned char[linkStatus];
    GLenum binaryFormat;
    glGetProgramBinaryOES(program, linkStatus, NULL, &binaryFormat, binary);
    if(outputToHeaderFile)
    {
        FILE *fp = fopen(outputFile, "w");
        fprintf(fp, "unsigned char %s[] = {\n", variableName);
        fprintf(fp, "%3i, ", binary[0]);
        for(int i = 1; i < linkStatus - 1; ++i)
        {
            if(i % 8 == 0)
                fprintf(fp, "\n");
            fprintf(fp, "%3i, ", binary[i]);
        }
        if((linkStatus - 1) % 8 == 0)
            fprintf(fp, "\n");
        fprintf(fp, "%3i\n};", binary[linkStatus - 1]);
        fclose(fp);
    }
    else
    {
        FILE *fp = fopen(outputFile, "wb");
        fwrite(binary, linkStatus, 1, fp);
        fclose(fp);
    }
    delete [] binary;
    printf("Compilation successful\n");
    return 0;
}

//
//   print usage to stdout
//
void usage()
{
    printf("Usage: glslPrecompiler [-o=[file] -a=[variable]] -v=[vertex_shader_1] ... -v=[vertex_shader_1] -f=[fragment_shader_1] ... -f=[fragment_shader_n]\n"
        "Where: filename : filename ending in .frag or .vert\n"
        "       -o=[file] : output file\n"
        "       -v=[variable] : output to char array named [variable] in header file, omit for binary file\n");
}

GLuint createShader(GLenum shaderType, const char **source, unsigned sourceCount)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, sourceCount, source, NULL);
    glCompileShader(shader);
    int compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

    if(compileStatus != GL_TRUE)
    {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &compileStatus);
        char *sourceBuffer = new char[compileStatus];
        char *shaderTypeName = (shaderType == GL_VERTEX_SHADER) ? "vertex shader(s)" : "fragment shader(s)";
        glGetShaderInfoLog(shader, compileStatus, NULL, sourceBuffer);
        printf("Failed to compile %s due to:\n%s\n", shaderTypeName, sourceBuffer);
        delete [] sourceBuffer;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
