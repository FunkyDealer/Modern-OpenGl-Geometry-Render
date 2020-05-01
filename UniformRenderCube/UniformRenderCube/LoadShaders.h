#include <GL\gl.h>

/*****************************************************************************
                     GLuint LoadShaders(ShaderInfo*);

DescriÁ„o:
----------
Recebe um array de estruturas ShaderInfo.
Cada estrutura contÈm:
- tipo de shader. No OpenGL 4.x poderÅEser um dos seguintes valores:
  - GL_COMPUTE_SHADER
  - GL_VERTEX_SHADER
  - GL_TESS_CONTROL_SHADER
  - GL_TESS_EVALUATION_SHADER
  - GL_GEOMETRY_SHADER
  - GL_FRAGMENT_SHADER
- apontador para uma C-string, que contÈm o nome do ficheiro com cÛdigo do shader
- valor que referencia o objeto shader criado

O array de estruturas deverÅEterminar com o valor GL_NONE no campo 'type'.
Exemplo:
ShaderInfo  shaders[] = {
	{ GL_VERTEX_SHADER, "triangles.vert" },
	{ GL_FRAGMENT_SHADER, "triangles.frag" },
	{ GL_NONE, NULL }
};

Retorno:
--------
Em caso de sucesso, a funcao retorna o valore que referencia o objeto programa.
Em caso de erro, serÅEretornado o valor zero (0).

*****************************************************************************/

// Descomentar para debug
#define _DEBUG

typedef struct {
	GLenum       type;
	const char*  filename;
	GLuint       shader;
} ShaderInfo;

GLuint LoadShaders(ShaderInfo*);
