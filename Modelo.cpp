//
// Created by alvaro on 21/10/26.
//

#include "Modelo.h"

#include <filesystem>
#include <stdexcept>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace PAG {

    /**
     * Constructor que carga un modelo desde un fichero y crea su VAO
     * @param ruta Ruta del fichero
     * @throws runtime_error si Assimp no puede cargar el fichero
     */
    Modelo::Modelo(const std::string& ruta) {
        nombre = std::filesystem::path(ruta).filename().string();

        Assimp::Importer importador;
        const aiScene* escena = importador.ReadFile(ruta,
                aiProcess_JoinIdenticalVertices | aiProcess_Triangulate
                | aiProcess_GenSmoothNormals);
        if (!escena || !escena->mRootNode) {
            throw std::runtime_error(importador.GetErrorString());
        }

        procesarNodo(escena->mRootNode, escena);
        creaVAO();
    }

    Modelo::~Modelo() {
        if (idVBO != 0) {
            glDeleteBuffers(1, &idVBO);
        }
        if (idIBO != 0) {
            glDeleteBuffers(1, &idIBO);
        }
        if (idVAO != 0) {
            glDeleteVertexArrays(1, &idVAO);
        }
    }

    /**
     * Recorre recursivamente los nodos de la escena y procesa sus mallas
     */
    void Modelo::procesarNodo(const aiNode* nodo, const aiScene* escena) {
        for (unsigned int i = 0; i < nodo->mNumMeshes; i++) {
            procesarMalla(escena->mMeshes[nodo->mMeshes[i]]);
        }
        for (unsigned int i = 0; i < nodo->mNumChildren; i++) {
            procesarNodo(nodo->mChildren[i], escena);
        }
    }

    /**
     * Añade los vértices y los índices de una malla a los del modelo,
     * así todas las mallas quedan en un único VAO
     */
    void Modelo::procesarMalla(const aiMesh* malla) {
        // Los índices de esta malla empiezan después de los vértices ya guardados
        unsigned int desplazamiento = vertices.size() / 6;

        for (unsigned int i = 0; i < malla->mNumVertices; i++) {
            vertices.push_back(malla->mVertices[i].x);
            vertices.push_back(malla->mVertices[i].y);
            vertices.push_back(malla->mVertices[i].z);
            vertices.push_back(malla->mNormals[i].x);
            vertices.push_back(malla->mNormals[i].y);
            vertices.push_back(malla->mNormals[i].z);
        }

        for (unsigned int i = 0; i < malla->mNumFaces; i++) {
            // Con Triangulate todas las caras son triángulos
            for (unsigned int j = 0; j < 3; j++) {
                indices.push_back(malla->mFaces[i].mIndices[j] + desplazamiento);
            }
        }
    }

    /**
     * Crea el VAO, el VBO entrelazado (posición y normal) y el IBO del modelo
     */
    void Modelo::creaVAO() {
        glGenVertexArrays(1, &idVAO);
        glBindVertexArray(idVAO);

        glGenBuffers(1, &idVBO);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                     vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                              (void*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &idIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                     indices.data(), GL_STATIC_DRAW);
    }

    /**
     * Dibuja el modelo con el shader program activo
     */
    void Modelo::dibujar() const {
        glBindVertexArray(idVAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    }

}
