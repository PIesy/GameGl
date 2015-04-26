#include "openglsdl.h"
#include <fstream>

class ShaderReader
{
    std::string* shader;
public:
    ShaderReader();
    void Analyze(std::string string);
    void* Fill();
};

class FileReader
{
public:
    FileReader(){}
    void* ReadByTemplate(std::string source, ShaderReader* scanner);
};

struct ShaderData
{
    const char** src;
    int* length;
    std::condition_variable finish;
    GLuint* shader;
    unsigned int type;
};

GlShader::GlShader(Worker *context)
{
    this->context = context;
}

void glDropShader(GLuint* shader)
{
    glDeleteShader(*shader);
    delete shader;
}

GlShader::~GlShader()
{
    GLuint* sh = new GLuint(shader);
    context->setTask(Task(glDropShader, sh));
}

void glShaderCreate(ShaderData* data)
{
    *data->shader = glCreateShader(data->type);
    glShaderSource(*data->shader, 1, data->src, data->length);
    glCompileShader(*data->shader);
    data->finish.notify_all();
}

void GlShader::Create(std::string source, unsigned int type)
{
    ShaderData data;
    std::unique_lock<std::mutex> lock(this->lock);

    data.length = &length;
    data.src = &src;
    data.type = type;
    data.shader = &shader;
    prepareShader(source);
    context->setTask(Task(glShaderCreate ,&data));
    data.finish.wait(lock);
    cleanup();
}

void glDestroyShader(GLuint* shader)
{
    glDeleteShader(*shader);
    delete shader;
}

void GlShader::Destroy()
{
    GLuint* sh = new GLuint(shader);
    context->setTask(Task(glDestroyShader, sh));
    shader = 0;
}

GlShader::operator GLuint()
{
    return shader;
}

void GlShader::prepareShader(std::string source)
{
    FileReader reader;
    ShaderReader scanner;

    str = (std::string*)reader.ReadByTemplate(source, &scanner);
    src = str->c_str();
    length = str->length();
}

void GlShader::cleanup()
{
    delete str;
    str = nullptr;
    src = nullptr;
    length = 0;
}

ShaderReader::ShaderReader()
{
    shader = new std::string;
}

void ShaderReader::Analyze(std::string string)
{
    shader->append(string);
    *shader += '\n';
}

void* ShaderReader::Fill()
{
    return shader;
}

void* FileReader::ReadByTemplate(std::string source, ShaderReader* scanner)
{
    std::fstream file;
    char buff[256];
    buff[255] = '\0';

    file.open(source);

    while(!file.eof())
    {
        file.getline(buff,255);
        scanner->Analyze(buff);
    }
    return scanner->Fill();
}
