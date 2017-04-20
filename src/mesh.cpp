#include "mesh.h"
#include <vector>
#include <iostream>
#include "obj_loader.h"

using std::cout;
using std::endl;
using std::cerr;
using std::ostringstream;

Mesh::Mesh(const std::string& fileName)
{
    IndexedModel model = OBJModel(fileName).ToIndexedModel();
    InitMesh(model);

}


Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
       
        IndexedModel model;

         for(unsigned int i=0; i < numVertices; i++)
        {
            model.positions.push_back(*vertices[i].GetPos());
            model.texCoords.push_back(*vertices[i].GetTextCoord());
            model.normals.push_back(*vertices[i].GetNormal());
            
        }


         for(unsigned int i=0; i < numIndices; i++)
        {
            model.indices.push_back( indices[i]);
        }

        InitMesh(model);


      
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_vertexArrayObject);
}


void Mesh::InitMesh(const IndexedModel& model)
{
        m_drawCount = model.indices.size();

        glGenVertexArrays(1, &m_vertexArrayObject);
        glBindVertexArray(m_vertexArrayObject);

        glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);

        glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
        glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1 , 2, GL_FLOAT, GL_FALSE, 0, 0);


        glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
        glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(model.normals[0]), &model.normals[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);


        glBindVertexArray(0);


}



void Mesh::Draw()
{

    glBindVertexArray(m_vertexArrayObject);

  //  glDrawArrays(GL_TRIANGLES, 0, m_drawCount);

    glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


// partie ocaml 

void finalize_mesh(value co) {
    cerr << __func__ << ": entered!" << endl;
    Mesh* w = Mesh_val(co);
    delete w;
  }

  // operations for the OCaml GC
  static struct custom_operations mesh_custom_ops = {"mesh_custom_ops", finalize_mesh, NULL, NULL, NULL, NULL};


extern "C"
{
CAMLprim value createMesh(value vertice)
{
    CAMLparam1(vertice);
    CAMLlocal3(head, vec3, vec2);
    int i,len, size;
   
    // pour recuperé la taille du tableau
    len = Wosize_val(vertice) / Double_wosize;
    Vertex *vertices;
    vertices = (Vertex*) malloc(sizeof(Vertex) * len);
  
    for( i = 0 ; i < len ; i++)
    {
     
     head = Field(vertice, i);

     vec3 = Field(head, 0);
     vec2 = Field(head,1);

   
     vertices[i] = Vertex( 
                            glm::vec3((float) Double_field(vec3, 0), (float) Double_field(vec3, 1),(float) Double_field(vec3, 2)),
                            glm::vec2((float) Double_field(vec2, 0), (float) Double_field(vec2, 1))
                         );
            
    }
       

    unsigned int indices[] = {0, 1, 2};

    Mesh* mesh;
    
    mesh = new Mesh(vertices, sizeof(vertices)/sizeof(vertices[0]), indices, sizeof(indices)/sizeof(indices[0]));
    value v = caml_alloc_custom(&mesh_custom_ops, sizeof(Mesh*), 0, 1);
    Mesh_val(v) = mesh;

    free(vertices);
    CAMLreturn(v);
}
}


extern "C"
{
CAMLprim value createMeshWithFile(value fileName)
{
    CAMLparam1(fileName);
    
    Mesh* mesh;
    
    mesh = new Mesh(String_val(fileName));
    value v = caml_alloc_custom(&mesh_custom_ops, sizeof(Mesh*), 0, 1);
    Mesh_val(v) = mesh;
    CAMLreturn(v);
}
}



extern "C"
{
CAMLprim value drawMesh(value msh)
{
    CAMLparam1(msh);
    Mesh* mesh = Mesh_val(msh);
    mesh->Draw();
    return Val_unit;
}
}