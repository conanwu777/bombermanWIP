//
// Created by hjeannin on 13/11/17.
//

#include "../inc/ObjectLoader.hpp"

ObjectLoader::ObjectLoader()
{
}

ObjectLoader::~ObjectLoader()
{
}

ObjectLoader::ObjectLoader(const char *filename)
{
    this->filename = filename;
}

bool ObjectLoader::parseObjectFile()
{
    std::ifstream in(filename, std::ios::in);
    if (!in)
    {
        std::cerr << "Cannot open " << filename << std::endl;
        return false;
    }

    std::string line;
    // Counting number of element.
    while (getline(in, line))
    {
        if (line.substr(0, 2) == "v ")
            vertex_array_num_elem++;
        else if (line.substr(0, 3) == "vt ")
            uv_array_num_elem++;
        else if (line.substr(0, 3) == "vn ")
            normal_array_num_elem++;
        else if (line.substr(0, 2) == "f ")
        {
            v_index_array_num_elem += 3;
            uv_index_array_num_elem += 3;
            n_index_array_num_elem += 3;
        }
        else if (line == "s 1")
            smooth_status = IS_SMOOTH;
        else if (line == "s off")
            smooth_status = IS_NOT_SMOOTH;
    }

    if (vertex_array_num_elem <= 0)
    {
        std::cerr << "Not enough vertex in " << this->filename << "." << std::endl;
        return false;
    }
    if (normal_array_num_elem <= 0)
    {
        std::cerr << "Not enough normal in " << this->filename << "." << std::endl;
        return false;
    }

    // Allocating for raw data.
    if (v_index_array_num_elem > 0)
        v_index_array = std::unique_ptr<GLuint[]>{ new GLuint[v_index_array_num_elem] };
    if (uv_index_array_num_elem > 0)
        uv_index_array = std::unique_ptr<GLuint[]>{ new GLuint[uv_index_array_num_elem] };
    if (n_index_array_num_elem > 0)
        n_index_array = std::unique_ptr<GLuint[]>{ new GLuint[n_index_array_num_elem] };
    if (vertex_array_num_elem > 0)
        vertex_array = std::unique_ptr<glm::vec3[]>{ new glm::vec3[vertex_array_num_elem] };
    if (uv_array_num_elem > 0)
        uv_array = std::unique_ptr<glm::vec2[]>{ new glm::vec2[uv_array_num_elem] };
    else
    {
        /// In case there is no UV, it sets UV to 0.0f 0.0f.
        uv_array = std::unique_ptr<glm::vec2[]>{ new glm::vec2[1] };
        uv_array[0].x = 0.0f;
        uv_array[0].y = 0.0f;
        uv_array_num_elem = 1;
        for (GLuint iter_uv_i_a = 0; iter_uv_i_a < uv_index_array_num_elem; ++iter_uv_i_a)
        {
            uv_index_array[iter_uv_i_a] = 1;
        }
    }
    if (normal_array_num_elem > 0)
        normal_array = std::unique_ptr<glm::vec3[]>{ new glm::vec3[normal_array_num_elem] };

    in.clear();                 // clear fail and eof bits
    in.seekg(0, std::ios::beg); // back to the start

    // Fill raw data from .obj
    int iter_vertex_array = 0;
    int iter_normal_array = 0;
    int iter_uv_array = 0;
    int iter_v_index = 0;
    int iter_n_index = 0;
    int iter_uv_index = 0;
    while (getline(in, line))
    {
        if (line.substr(0, 2) == "v ")
        {
            std::istringstream s(line.substr(2));
            s >> vertex_array[iter_vertex_array].x;
            s >> vertex_array[iter_vertex_array].y;
            s >> vertex_array[iter_vertex_array].z;
            iter_vertex_array++;
        }
        else if (line.substr(0, 3) == "vt ")
        {
            std::istringstream s(line.substr(3));
            s >> uv_array[iter_uv_array].x;
            s >> uv_array[iter_uv_array].y;
            iter_uv_array++;
        }
        else if (line.substr(0, 3) == "vn ")
        {
            std::istringstream s(line.substr(3));
            s >> normal_array[iter_normal_array].x;
            s >> normal_array[iter_normal_array].y;
            s >> normal_array[iter_normal_array].z;
            iter_normal_array++;
        }
        else if (line.substr(0, 2) == "f ")
        {
            std::istringstream s(line.substr(2));
            for (int i = 0; i < 3; ++i)
            {
                s >> v_index_array[iter_v_index];
                iter_v_index++;
                if (s.peek() == '/')
                    s.ignore();
                else
                    std::cerr << "Sumg Ting Wong" << std::endl;
                /// Case where there is no UV fill index with 1.
                if (s.peek() != '/')
                {
                    s >> uv_index_array[iter_uv_index];
                    iter_uv_index++;
                }
                if (s.peek() == '/')
                    s.ignore();
                else
                    std::cerr << "Sumg Ting Wong" << std::endl;
                s >> n_index_array[iter_n_index];
                iter_n_index++;
            }
        }
    }
//    printParsedData();
    return true;
}

void ObjectLoader::fitIndexesToOpenGL()
{
    // Fix faces start from 0 in OpenGL (start at 1 in .obj)
    for (GLuint iter_v_index = 0; iter_v_index < v_index_array_num_elem; ++iter_v_index)
        v_index_array[iter_v_index] -= 1;
    for (GLuint iter_uv_index = 0; iter_uv_index < uv_index_array_num_elem; ++iter_uv_index)
        uv_index_array[iter_uv_index] -= 1;
    for (GLuint iter_n_index = 0; iter_n_index < n_index_array_num_elem; ++iter_n_index)
        n_index_array[iter_n_index] -= 1;
}

void ObjectLoader::createFATInterleavedData()
{
    GLuint iter_indexes = 0;
    interleaved_fat_data_num_elem = v_index_array_num_elem * 8;
    interleaved_fat_data_mem_size = sizeof(GLfloat) * interleaved_fat_data_num_elem;
    interleaved_fat_data_array = std::unique_ptr<GLfloat[]>{ new GLfloat[interleaved_fat_data_num_elem] };
    for (GLuint iter_ifda = 0; iter_ifda < interleaved_fat_data_num_elem; iter_ifda += 8)
    {
        interleaved_fat_data_array[iter_ifda + 0] = vertex_array[v_index_array[iter_indexes]].x;
        interleaved_fat_data_array[iter_ifda + 1] = vertex_array[v_index_array[iter_indexes]].y;
        interleaved_fat_data_array[iter_ifda + 2] = vertex_array[v_index_array[iter_indexes]].z;
        interleaved_fat_data_array[iter_ifda + 3] = uv_array[uv_index_array[iter_indexes]].x;
        interleaved_fat_data_array[iter_ifda + 4] = uv_array[uv_index_array[iter_indexes]].y;
        interleaved_fat_data_array[iter_ifda + 5] = normal_array[n_index_array[iter_indexes]].x;
        interleaved_fat_data_array[iter_ifda + 6] = normal_array[n_index_array[iter_indexes]].y;
        interleaved_fat_data_array[iter_ifda + 7] = normal_array[n_index_array[iter_indexes]].z;
        iter_indexes++;
    }
}

void ObjectLoader::makeOpenGLBuffer()
{
    if (parseObjectFile())
    {
        fitIndexesToOpenGL();
        if (smooth_status == IS_NOT_SMOOTH)
        {
            createFATInterleavedData();
            data_formatting_status = INTERLEAVED_FAT_DATA;
        }
        else if (smooth_status == IS_SMOOTH)
        {
            createIndexedInterleavedData();
            data_formatting_status = INDEXED_INTERLEAVED_DATA;
        }
        else
        {
            std::cerr << "ObjectLoader: Can not determine how to store data." << std::endl;
        }
    }
}

void ObjectLoader::printParsedData()
{
    if (vertex_array != nullptr && uv_array != nullptr && normal_array != nullptr)
    {
        std::cout << "Parsed data for: " << filename << std::endl;
        std::cout << "Vertex (" << vertex_array_num_elem << "):" << std::endl;
        for (GLuint iter_v_a = 0; iter_v_a < vertex_array_num_elem; iter_v_a++)
        {
            std::cout << vertex_array[iter_v_a].x << " "
                      << vertex_array[iter_v_a].y << " "
                      << vertex_array[iter_v_a].z << " | ";
        }
        std::cout << std::endl << std::endl;
        std::cout << "UV (" << uv_array_num_elem << "):" << std::endl;
        for (GLuint iter_uv_a = 0; iter_uv_a < uv_array_num_elem; iter_uv_a++)
        {
            std::cout << uv_array[iter_uv_a].x << " "
                      << uv_array[iter_uv_a].y << " | ";
        }
        std::cout << std::endl << std::endl;
        std::cout << "Normal (" << normal_array_num_elem << "):" << std::endl;
        for (GLuint iter_n_a = 0; iter_n_a < normal_array_num_elem; iter_n_a++)
        {
            std::cout << normal_array[iter_n_a].x << " "
                      << normal_array[iter_n_a].y << " "
                      << normal_array[iter_n_a].z << " | ";
        }
        std::cout << std::endl << std::endl;
        std::cout << "Vertex Index: (" << v_index_array_num_elem << "):" << std::endl;
        for (GLuint iter_v_i_a = 0; iter_v_i_a < v_index_array_num_elem; iter_v_i_a++)
        {
            std::cout << v_index_array[iter_v_i_a] << " ";
        }
        std::cout << std::endl << std::endl;
        std::cout << "UV Index: (" << uv_index_array_num_elem << "):" << std::endl;
        for (GLuint iter_uv_i_a = 0; iter_uv_i_a < uv_index_array_num_elem; iter_uv_i_a++)
        {
            std::cout << uv_index_array[iter_uv_i_a] << " ";
        }
        std::cout << std::endl << std::endl;
        std::cout << "Normal Index: (" << n_index_array_num_elem << "):" << std::endl;
        for (GLuint iter_n_i_a = 0; iter_n_i_a < n_index_array_num_elem; iter_n_i_a++)
        {
            std::cout << n_index_array[iter_n_i_a] << " ";
        }
        std::cout << std::endl << std::endl;
    }
}

void ObjectLoader::createIndexedInterleavedData()
{
    iid.num_elem = vertex_array_num_elem * 8;
    iid.array = std::unique_ptr<GLfloat[]>{ new GLfloat[iid.num_elem] };
    iid.mem_size = sizeof(GLfloat) * iid.num_elem;
    iid.stride = sizeof(GLfloat) * 8;
    iid.index_num_elem = v_index_array_num_elem;
    iid.index = std::unique_ptr<GLuint[]>{ new GLuint[iid.index_num_elem] };
    iid.index_mem_size = sizeof(GLuint) * iid.index_num_elem;

    GLuint iter_index = 0;
    while (iter_index < v_index_array_num_elem)
    {
        iid.array[v_index_array[iter_index] * 8 + 0] = vertex_array[v_index_array[iter_index]].x;
        iid.array[v_index_array[iter_index] * 8 + 1] = vertex_array[v_index_array[iter_index]].y;
        iid.array[v_index_array[iter_index] * 8 + 2] = vertex_array[v_index_array[iter_index]].z;

        iid.array[v_index_array[iter_index] * 8 + 3] = uv_array[uv_index_array[iter_index]].x;
        iid.array[v_index_array[iter_index] * 8 + 4] = uv_array[uv_index_array[iter_index]].y;

        iid.array[v_index_array[iter_index] * 8 + 5] = normal_array[n_index_array[iter_index]].x;
        iid.array[v_index_array[iter_index] * 8 + 6] = normal_array[n_index_array[iter_index]].y;
        iid.array[v_index_array[iter_index] * 8 + 7] = normal_array[n_index_array[iter_index]].z;
        iter_index++;
    }
    /// Copying index VERY DIRTY! need to change pointer and dont forget to NOT freeMemory
    for (GLuint iter_v_index = 0; iter_v_index < v_index_array_num_elem; ++iter_v_index)
    {
        iid.index[iter_v_index] = v_index_array[iter_v_index];
    }
//    printIIDArray();
}

void ObjectLoader::printIIDArray()
{
    std::cout << std::endl;
    for (GLuint iter_iid_array = 0; iter_iid_array < iid.num_elem; iter_iid_array += 8)
    {
        std::cout << iter_iid_array / 8 << " "
                  << "V[" << iid.array[iter_iid_array + 0]
                  << ", " << iid.array[iter_iid_array + 1]
                  << ", " << iid.array[iter_iid_array + 2]
                  << "] UV[" << iid.array[iter_iid_array + 3]
                  << ", " << iid.array[iter_iid_array + 4]
                  << "] N[" << iid.array[iter_iid_array + 5]
                  << ", " << iid.array[iter_iid_array + 6]
                  << ", " << iid.array[iter_iid_array + 7] << "]" << std::endl;
    }

}

