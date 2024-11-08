//     Universidade Federal do Rio Grande do Sul
//             Instituto de Informática
//       Departamento de Informática Aplicada
//
//    INF01047 Fundamentos de Computação Gráfica
//               Prof. Eduardo Gastal
//
//                   LABORATÓRIO 5
//

// Arquivos "headers" padrões de C podem ser incluídos em um
// programa C++, sendo necessário somente adicionar o caractere
// "c" antes de seu nome, e remover o sufixo ".h". Exemplo:
//    #include <stdio.h> // Em C
//  vira
//    #include <cstdio> // Em C++
//
#include <cmath>
#include <cstdio>
#include <cstdlib>

// Headers abaixo são específicos de C++
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Headers da biblioteca para carregar modelos obj
#include <tiny_obj_loader.h>

#include <stb_image.h>

// Headers locais, definidos na pasta "include/"
#include "utils.h"
#include "matrices.h"
#include "collisions.cpp"

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

// Identificador que define qual objeto está sendo desenhado no momento
#define CAMPFIRE  0
#define FIRE 1
#define CAVE 2
#define CAVE_WALLS 3
#define CAVE_FLOOR 4
#define GRUTA_BASE 5
#define GREEK_BODY 6
#define GREEK_HEAD 7
#define TITLE 8
#define PRISIONER 9
#define PRISIONER_EYES 10
#define PRISIONER_TEETH 11
#define PRISIONER_ROCK 12
#define PRISIONER_CHAIN 13
#define LADDER 14
#define GREEK2 15
#define MOUNTAIN 16
#define CAVE_ENTRANCE1 17
#define CAVE_ENTRANCE2 18
#define RABBIT 19
#define BEAR 20
#define LETTERING 21
#define ENDING1 22
#define ENDING2 23
#define ENDING3 24
#define ENDING4 25
#define ENDING5 26
#define ENDING6 27
#define ENDING7 28
#define ENDING8 29
#define ENDING9 30


// Estrutura que representa um modelo geométrico carregado a partir de um
// arquivo ".obj". Veja https://en.wikipedia.org/wiki/Wavefront_.obj_file .


// Declaração de funções utilizadas para pilha de matrizes de modelagem.
void PushMatrix(glm::mat4 M);
void PopMatrix(glm::mat4& M);


// Declarações de funções adicionadas para o trabalho Final
void AddPlayer(glm::mat4 model);
void MovePlayer();
void AddCave();
void AddCavePhysics();
void AddLadder(glm::mat4 model);
bool g_LadderCollision = false;
void AddTitle(glm::mat4 model);
void AddPrisioner(glm::mat4 model, bool showBody, bool showRest);
bool g_PrisionerRockCollision = false;
void AddCampfire(glm::mat4 model);
void AddGreek2(glm::mat4 model, GLFWwindow* window);
bool g_Greek2Collision = false;
void AddCaveEntrance(glm::mat4 model);
void AddMountain(glm::mat4 model);
void AddRabbit(glm::mat4 model);
void AddBear(glm::mat4 model);
void AddLettering(glm::mat4 model, int ending);

bool interact_radius(glm::mat4 model, char* object, float radius_expand);

void TextRendering_ShowInitialScreenText(GLFWwindow* window, char* mensagem, float scale);
void TextRendering_ShowChatCharacters(GLFWwindow* window, char* mensagem, float scale, float position);
void TextRendering_ShowInferiorCommand(GLFWwindow* window, char* mensagem, float scale);

glm::vec4 bezier_curve_two_degree(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, float t);


// Declaração de várias funções utilizadas em main().  Essas estão definidas
// logo após a definição de main() neste arquivo.

void DrawCube(GLint render_as_black_uniform); // Desenha um cubo
GLuint BuildTriangles(); // Constrói triângulos para renderização

void BuildTrianglesAndAddToVirtualScene(ObjModel*); // Constrói representação de um ObjModel como malha de triângulos para renderização
void ComputeNormals(ObjModel* model); // Computa normais de um ObjModel, caso não existam.
void LoadShadersFromFiles(); // Carrega os shaders de vértice e fragmento, criando um programa de GPU
void LoadTextureImage(const char* filename, const char* type); // Função que carrega imagens de textura
void DrawVirtualObject(const char* object_name); // Desenha um objeto armazenado em g_VirtualScene
GLuint LoadShader_Vertex(const char* filename);   // Carrega um vertex shader
GLuint LoadShader_Fragment(const char* filename); // Carrega um fragment shader
void LoadShader(const char* filename, GLuint shader_id); // Função utilizada pelas duas acima
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id); // Cria um programa de GPU
void PrintObjModelInfo(ObjModel*); // Função para debugging

// Declaração de funções auxiliares para renderizar texto dentro da janela
// OpenGL. Estas funções estão definidas no arquivo "textrendering.cpp".
void TextRendering_Init();
float TextRendering_LineHeight(GLFWwindow* window);
float TextRendering_CharWidth(GLFWwindow* window);
void TextRendering_PrintString(GLFWwindow* window, const std::string &str, float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrix(GLFWwindow* window, glm::mat4 M, float x, float y, float scale = 1.0f);
void TextRendering_PrintVector(GLFWwindow* window, glm::vec4 v, float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrixVectorProduct(GLFWwindow* window, glm::mat4 M, glm::vec4 v, float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrixVectorProductMoreDigits(GLFWwindow* window, glm::mat4 M, glm::vec4 v, float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrixVectorProductDivW(GLFWwindow* window, glm::mat4 M, glm::vec4 v, float x, float y, float scale = 1.0f);

// Funções abaixo renderizam como texto na janela OpenGL algumas matrizes e
// outras informações do programa. Definidas após main().
void TextRendering_ShowModelViewProjection(GLFWwindow* window, glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec4 p_model);
void TextRendering_ShowEulerAngles(GLFWwindow* window);
void TextRendering_ShowFramesPerSecond(GLFWwindow* window);

// Funções callback para comunicação com o sistema operacional e interação do
// usuário. Veja mais comentários nas definições das mesmas, abaixo.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ErrorCallback(int error, const char* description);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

// Abaixo definimos variáveis globais utilizadas em várias funções do código.

// A cena virtual é uma lista de objetos nomeados, guardados em um dicionário
// (map).  Veja dentro da função BuildTrianglesAndAddToVirtualScene() como que são incluídos
// objetos dentro da variável g_VirtualScene, e veja na função main() como
// estes são acessados.
std::map<std::string, SceneObject> g_VirtualScene;

// Pilha que guardará as matrizes de modelagem.
std::stack<glm::mat4>  g_MatrixStack;

// Razão de proporção da janela (largura/altura). Veja função FramebufferSizeCallback().
float g_ScreenRatio = 1.0f;

// Ângulos de Euler que controlam a rotação de um dos cubos da cena virtual
float g_AngleY_torso        = 0.0f;
float g_AngleX_leg          = 0.0f;
bool movement_AngleX_leg    = true;

// "g_LeftMouseButtonPressed = true" se o usuário está com o botão esquerdo do mouse
// pressionado no momento atual. Veja função MouseButtonCallback().
bool g_LeftMouseButtonPressed = false;
bool g_RightMouseButtonPressed = false; // Análogo para botão direito do mouse
bool g_MiddleMouseButtonPressed = false; // Análogo para botão do meio do mouse

// "g_?KeyPressed = true" se o usuário está com a tecla ?
// pressionada no momento atual.
bool g_WKeyPressed = false;
bool g_AKeyPressed = false;
bool g_SKeyPressed = false;
bool g_DKeyPressed = false;
bool g_COMMAKeyPressed = false;
bool g_PERIODKeyPressed = false;

// Variáveis que definem a câmera em coordenadas esféricas, controladas pelo
// usuário através do mouse (veja função CursorPosCallback()). A posição
// efetiva da câmera é calculada dentro da função main(), dentro do loop de
// renderização.
float g_CameraTheta = M_PI_2; // Ângulo no plano ZX em relação ao eixo Z
float g_CameraPhi = 0.0f;   // Ângulo em relação ao eixo Y
float g_CameraDistance = 6.0f; // Distância da câmera para a origem

// Variáveis que controlam rotação do antebraço
float g_ForearmAngleZ = 0.0f;
float g_ForearmAngleX = 0.0f;

// Variáveis que controlam translação do torso
float g_TorsoPositionX = 7.0f;
float g_TorsoPositionY = 0.0f;
float g_TorsoPositionZ = 25.0f;

float out_caveX = -60.0f;
float out_caveY = 0.5f;
float out_caveZ = 410.0f;

float current_time;
float delta_t;
float fast_speed = 8.0f;
float normal_speed = 5.0f;
float speed = normal_speed;
float spin_speed = 3.0f;
float prev_time;

//Variável que indica se o jogo está na tela inicial ou não
bool g_InitialScreen = true;
bool g_InitialScreen_FirstTime = true;

//Variável que indica se o jogador está fora da caverna ou não
bool g_OutCave = false;

//Variáveis de telas finais
bool g_ShowEnding = false;
int g_Ending = 0;

//Variáveis de chat
bool g_FirstLettering = false;
bool g_ShowChat = false;
bool g_ShowFirstChat = false;

//Primeira conversa com o prisioneiro -----------------------------------------------------------------
bool g_ShowPrisionerFirstChat = false;

int PrisionerFirstChatID = 0;
int PrisionerFirstChatMAX = 4;
char PrisionerFirstChat[][150] = {
    {"(Prisioneiro) => Ei, cara, como voce foi parar ai embaixo?"},
    {"(Jogador) => Eu nao sei, eu acordei aqui."},
    {"(Prisioneiro) => Caramba, cara, sobe dai. Os monstros vao te matar assim como fizeram com o Pericles."},
    {"[1] Voltar para cima      [2] Continuar embaixo"}
};

bool PrisionerFirstChatChoose = false;
int PrisionerFirstChatOption = 0;

char PrisionerFirstChatOption1[] = "(Jogador) => Tem razao... Acho que eh melhor voltar pra cima.";

int PrisionerFirstChatOption2ID = 0;
int PrisionerFirstChatOption2MAX = 2;
char PrisionerFirstChatOption2[][150] = {
    {"(Jogador) => Sei nao, Prometheus, nao parece perigoso."},
    {"(Prisioneiro) => Voce que sabe... Eu vou ficar aqui."}
};

//Primeira conversa com Pericles -------------------------------------------------------------------------
bool g_ShowPericlesFirstChat = false;

int PericlesFirstChatID = 0;
int PericlesFirstChatMAX = 21;
char PericlesFirstChat[][150] = {
    {"(Jogador) => Pericles! Voce esta vivo! Quem diria!"},
    {"(Pericles) => Ahn... Eu diria. Hehe."},
    {"(Jogador) => Foi voce que me libertou?"},
    {"(Pericles) => Claro, e de nada, a proposito. Hehe."},
    {"(Jogador) => Mas por que eu?"},
    {"(Pericles) => Porque eu gosto de voce! Haha! Pelo que mais seria?"},
    {"(Jogador) => Entao voce nao ficou chateado por eu ficar zombando de voce durante 40 anos seguidos, todos os dias e noites, sem excecao?"},
    {"(Pericles) => Pfff... Claro que nao, sao aguas passadas. Amor Vincit Omnia."},
    {"(Jogador) => O que voce disse?"},
    {"(Pericles) => Ah, eh uma expressao que vai ficar famosa no futuro. Eu ainda nao sei o que significa."},
    {"(Jogador) => Certo..."},
    {"(Pericles) => Mas nao importa, agora voce tem que sair da caverna tambem!"},
    {"(Jogador) => Espera, como assim?"},
    {"(Pericles) => Ora, eu te libertei! Voce tem que sair desse lugar! Desbravar o mundo!"},
    {"(Jogador) => Mas nao eh perigoso?"},
    {"(Pericles) => Pfff... Claro que nao! Eh so voce ir la fora, sozinho, desarmado, e explorar!"},
    {"(Jogador) => Nao sei, voce esta meio estranho."},
    {"(Pericles) => Que nada! So estou um pouco mudado! Eu nao sou mais o Pericles que voce INFERNIZOU DURANTE 40 ANOS SEGUIDOS. Hehe. Va logo."},
    {"(Jogador) => Espera, voce esta armado?"},
    {"(Pericles) => Nao, nao, isso nao eh nada, fique tranquilo..."},
    {"[1] Matar Pericles e Roubar Arma      [2] Roubar Arma     [3] Obedecer"}
};

bool PericlesFirstChatChoose = false;
int PericlesFirstChatOption = 0;
bool g_PericlesKilled = false;
bool g_Weapon = false;
float startTimeDecision = 0.0;
float timeDecision = 5.0; //tempo disponível para o jogador fazer sua escolha

char PericlesFirstChatOption1[] = "(Pericles) => Ahhhhhh.... fui assassinado!!";

int PericlesFirstChatOption2ID = 0;
int PericlesFirstChatOption2MAX = 3;
char PericlesFirstChatOption2[][150] = {
    {"(Pericles) => Desgracado!!"},
    {"(Jogador) => Que bela espada voce tem meu amigo."},
    {"(Pericles) => Bom, tudo bem, faca como quiser! Agora ve se me deixa em paz."}
};

int PericlesFirstChatOption3ID = 0;
int PericlesFirstChatOption3MAX = 2;
char PericlesFirstChatOption3[][150] = {
    {"(Jogador) => Certo, acho que vou seguir seu conselho."},
    {"(Pericles) => Otimo! Hehe. Que os Deuses o acompanhem! Ou melhor, que Hades o acompanhe."}
};

char PericlesFirstChatOption4[] = "(Pericles) => Foi mal, voce nao devia ter visto isso.";


//Conversa fora da caverna sem arma -------------------------------------------------------------------------
bool g_ShowOutCaveNoWeaponChat = false;
int g_ShowBunny = 2; //Em qual frase o coelho começa a se mover
int g_ShowBear = 5; //Em qual frase o urso começa a se mover

int OutCaveNoWeaponChatID = 0;
int OutCaveNoWeaponChatMAX = 7;
char OutCaveNoWeaponChat[][150] = {
    {"(Jogador) => Caramba, quanta luz! Mas acho que ja estou me acostumando."},
    {"(Jogador) => Eh tao lindo aqui fora, e tao diferente! Isso eh definitivamente um momento mind blowing!"},
    {"(Jogador) => Meu mundo esta se expandindo agora. Sou uma nova pessoa, e vou buscar mais conhecimento nos anos a frente!"},
    {"(Jogador) => AAAAAHHH! O que eh aquilo?!"},
    {"(Jogador) => Ah... Que fofinho... E pensar que a sua sombra me deixava cheio de medo. Sera que todos os monstros daqui de fora sao assim?"},
    {"(Coelho) => Olha pra tras idiota."},
    {"(Jogador) => Mas o que?!"}
};

//Conversa fora da caverna com arma -------------------------------------------------------------------------
bool g_ShowOutCaveWeaponChat = false;
bool g_PlayerKilled = false;

int OutCaveWeaponChatID = 0;
int OutCaveWeaponChatMAX = 10;
char OutCaveWeaponChat[][150] = {
    {"(Jogador) => Caramba, quanta luz! Mas acho que ja estou me acostumando."},
    {"(Jogador) => Eh tao lindo aqui fora, e tao diferente! Isso eh definitivamente um momento mind blowing!"},
    {"(Jogador) => Meu mundo esta se expandindo agora. Sou uma nova pessoa, e vou buscar mais conhecimento nos anos a frente!"},
    {"(Jogador) => AAAAAHHH! O que eh aquilo?!"},
    {"(Jogador) => Ah... Que fofinho... E pensar que a sua sombra me deixava cheio de medo. Sera que todos os monstros daqui de fora sao assim?"},
    {"(Coelho) => Me chama de fofinho de novo e eu te mostro um verdadeiro monstro."},
    {"(Jogador) => ..."},
    {"(Jogador) => Muito bem. Sera que eu devo voltar e libertar meu amigo Prometheus, ou ficar e explorar esse mundao?"},
    {"(Coelho) => Ta falando com quem idiota? Ta doido eh?"},
    {"[1] Ficar e Explorar      [2] Matar Coelho        [3] Voltar para a Caverna"}
};

bool OutCaveWeaponChatChoose = false;
int OutCaveWeaponChatOption = 0;

char OutCaveWeaponChatOption1[] = "(Jogador) => Prometheus que se dane! Eu vou embora daqui. Adeus animal... ahn... legalzinho.";

char OutCaveWeaponChatOption2[] = "(Jogador) => Quer saber? Cansei de voce FOFINHO!!!";

char OutCaveWeaponChatOption3[] = "(Jogador) => Acho que vou libertar meu amigo Prometheus.";


//Segunda conversa com o prisioneiro -----------------------------------------------------------------
bool g_ShowPrisionerSecondChat = false;
bool g_isSecondChat = false;

int PrisionerSecondChatID = 0;
int PrisionerSecondChatMAX = 2;
char PrisionerSecondChat[][150] = {
    {"(Prometheus) => Voce esta vivo, que bom! Achei que os monstros o haviam matado! Como sobreviveu?"},
    {"[1] Nao encontrei nenhum monstro.      [2] Nao ha monstros."}
};

bool PrisionerSecondChatChoose = false;
int PrisionerSecondChatOption = 0;
int g_FreePrisioner = 6; //Em qual frase o prisioneiro é libertado

int PrisionerSecondChatOption1ID = 0;
int PrisionerSecondChatOption1MAX = 10;
char PrisionerSecondChatOption1[][150] = {
    {"(Jogador) => Eu sai deste lugar e nao encontrei nenhum monstro. Estamos seguros por aqui."},
    {"(Prisioneiro) => Entao eles foram embora? Mas sera que nao podem voltar?"},
    {"(Jogador) => Fique tranquilo, Prometheus, eu tenho uma arma que pode mata-los."},
    {"(Prisioneiro) => Serio? Entao acho que me sinto mais seguro."},
    {"(Jogador) => Quer que eu te tire dai?"},
    {"(Prisioneiro) => Ahn... ta, acho que sim. Se esta seguro."},
    {"(Jogador) => Por que voce ainda esta com o braco levantado?"},
    {"(Prisioneiro) => Droga. Acho que eu fiquei tanto tempo assim que eu nao consigo mais baixar."},
    {"(Jogador) => Ugh... A primeira coisa que vamos fazer eh lavar esse seu suvaco."},
    {"(Prisioneiro) => Acho melhor mesmo."}
};

int PrisionerSecondChatOption2ID = 0;
int PrisionerSecondChatOption2MAX = 9;
char PrisionerSecondChatOption2[][150] = {
    {"(Jogador) => Nao ha monstros, Prometheus, eh tudo mentira."},
    {"(Prisioneiro) => O que? Como assim?"},
    {"(Jogador) => Os monstros que nos viamos sao apenas sombras de uma fogueira. Ha um mundo inteiro la fora."},
    {"(Prisioneiro) => Nao, nao, voce esta mentindo..."},
    {"(Jogador) => Espera, vou tirar voce dai."},
    {"(Prisioneiro) => Nao! Va embora! Me deixe aqui! Voce deve ser um monstro tambem! Meu amigo deve estar morto! Saia daqui monstro, saia!"},
    {"(Jogador) => Mas Prometheus..."},
    {"(Prisioneiro) => Va embora!"},
    {"(Jogador) => OK... Se voce diz..."}
};


bool movement_restricted = false;

float movement_rabbit_part = 0.0f; // Movimento do coelho em uma curva de bezier indo de 0 a 1
float movement_bear_part = 0.0f; // Movimento do urso em uma curva de bezier indo de 0 a 1
float movement_player_part = 0.0f; // Movimento do player em uma curva de bezier indo de 0 a 1

glm::vec4 movement_normal = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

// Variável que controla se o texto informativo será mostrado na tela.
bool g_ShowInfoText = true;

// Variáveis que definem um programa de GPU (shaders). Veja função LoadShadersFromFiles().
GLuint g_GpuProgramID = 0;
GLint g_model_uniform;
GLint g_view_uniform;
GLint g_projection_uniform;
GLint g_object_id_uniform;
GLint g_bbox_min_uniform;
GLint g_bbox_max_uniform;

glm::vec4 camera_position_c;
glm::vec4 camera_lookat_l;
glm::vec4 camera_view_vector;
glm::vec4 camera_up_vector;
glm::vec4 w;
glm::vec4 u;
bool free_camera = false;

// Número de texturas carregadas pela função LoadTextureImage()
GLuint g_NumLoadedTextures = 0;


// Construímos a representação de objetos geométricos através de malhas de triângulos
ObjModel cavemodel("../../data/cave/cave.obj");
ObjModel campfiremodel("../../data/campfire/Campfire.obj");
ObjModel greekmodel("../../data/greek/greek.obj");
ObjModel prisionermodel("../../data/prisioner/prisioner.obj");
ObjModel grutamodel("../../data/gruta/gruta.obj");
ObjModel laddermodel("../../data/ladder/ladder.obj");
ObjModel titlemodel("../../data/title/title.obj");
ObjModel greek2model("../../data/greek2/greek2.obj");
ObjModel caveEntrancemodel("../../data/cave_entrance/cave_entrance.obj");
ObjModel mountainmodel("../../data/mountain/mountain.obj");
ObjModel rabbitmodel("../../data/rabbit/rabbit.obj");
ObjModel bearmodel("../../data/bear/bear.obj");
ObjModel letteringmodel("../../data/lettering/lettering.obj");


int main(int argc, char* argv[])
{
    // Inicializamos a biblioteca GLFW, utilizada para criar uma janela do
    // sistema operacional, onde poderemos renderizar com OpenGL.
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos o callback para impressão de erros da GLFW no terminal
    glfwSetErrorCallback(ErrorCallback);

    // Pedimos para utilizar OpenGL versão 3.3 (ou superior)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    glfwWindowHint(GLFW_SAMPLES,16); //Multisample anti-aliasing
    //glfwWindowHint(GLFW_DECORATED, NULL); //Remove as bordas da janela
    // Pedimos para utilizar o perfil "core", isto é, utilizaremos somente as
    // funções modernas de OpenGL.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criamos uma janela do sistema operacional, com 800 colunas e 600 linhas
    // de pixels, e com título "INF01047 ...".
    GLFWwindow* window;

    window = glfwCreateWindow(glfwGetVideoMode(glfwGetPrimaryMonitor())->width,
                              glfwGetVideoMode(glfwGetPrimaryMonitor())->height,
                              "Myth of the Cave", glfwGetPrimaryMonitor(), NULL); // Full screen

    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos a função de callback que será chamada sempre que o usuário
    // pressionar alguma tecla do teclado ...
    glfwSetKeyCallback(window, KeyCallback);
    // ... ou clicar os botões do mouse ...
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    // ... ou movimentar o cursor do mouse em cima da janela ...
    glfwSetCursorPosCallback(window, CursorPosCallback);
    // ... ou rolar a "rodinha" do mouse.
    glfwSetScrollCallback(window, ScrollCallback);

    // Indicamos que as chamadas OpenGL deverão renderizar nesta janela
    glfwMakeContextCurrent(window);

    // Carregamento de todas funções definidas por OpenGL 3.3, utilizando a
    // biblioteca GLAD.
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // Definimos a função de callback que será chamada sempre que a janela for
    // redimensionada, por consequência alterando o tamanho do "framebuffer"
    // (região de memória onde são armazenados os pixels da imagem).
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    FramebufferSizeCallback(window,
                            glfwGetVideoMode(glfwGetPrimaryMonitor())->width,
                            glfwGetVideoMode(glfwGetPrimaryMonitor())->height); // Forçamos a chamada do callback acima, para definir g_ScreenRatio.

    // Imprimimos no terminal informações sobre a GPU do sistema
    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *glversion   = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

    // Carregamos os shaders de vértices e de fragmentos que serão utilizados
    // para renderização. Veja slides 180-200 do documento Aula_03_Rendering_Pipeline_Grafico.pdf.
    //
    LoadShadersFromFiles();

    // Carregamos duas imagens para serem utilizadas como textura
    LoadTextureImage("../../data/campfire/Campfire_MAT_BaseColor_01.jpg",""); // TextureImageCampfire
    LoadTextureImage("../../data/campfire/Campfire_fire_MAT_BaseColor_Alpha_white.png",""); // TextureImageFire
    LoadTextureImage("../../data/cave/RGB_e34762cfafeb488aa5ba2f697743a0bc_Rock_Kit_C.png",""); // TextureImageCaveWalls
    LoadTextureImage("../../data/cave/cave-floor-rock_albedo.png",""); // TextureImageCaveFloor
    LoadTextureImage("../../data/gruta/GRUTA_BASE_defaultMat_BaseColor.png",""); // TextureImageGruta
    LoadTextureImage("../../data/greek/RGB_a667b4b148b141cbbe618925eb411f31_07_aristotle_body.tga.png",""); // TextureImageGreekBody
    LoadTextureImage("../../data/greek/RGB_6c497cd7818a44f79491a2e4a0e8c8af_07_aristotle_head.tga.png",""); // TextureImageGreekHead
    LoadTextureImage("../../data/title/yellow-wall-texture-with-scratches.jpg",""); // TextureImageTitle
    LoadTextureImage("../../data/prisioner/667a8017571041f5bb6c01ebdb1137b7_RGB_Promitheus_diffuse.jpeg",""); // TextureImagePrisioner
    LoadTextureImage("../../data/prisioner/63f774b953eb4321a0fd1f09e8419fc0_RGB_EYES_blues.jpeg",""); // TextureImagePrisionerEyes
    LoadTextureImage("../../data/prisioner/eebbf88e369c458f9a44690ec444a16e_RGB_Aset_rock_sandstone_M_pjbqZ_2K_Albedo.jpeg",""); // TextureImagePrisionerRock
    LoadTextureImage("../../data/ladder/ladder_material_baseColor.png",""); // TextureImageLadder
    LoadTextureImage("../../data/greek2/RGB_379bb9ffa60c48d994f2b3773c413794_Pericles_albedo.jpeg",""); // TextureImageGreek2
    LoadTextureImage("../../data/mountain/RGB_b2bf6ec020774c3e8b09917a167753a5_MountainAlbedo4096.jpg",""); // TextureImageMountain
    LoadTextureImage("../../data/mountain/N_4e24f5b6ede843009d3a281f6258ce2a_MountainNormal4096.jpeg","normal"); // TextureNormalMountain
    LoadTextureImage("../../data/cave_entrance/cave_albedo.jpg",""); // TextureImageCaveEntrance1
    LoadTextureImage("../../data/cave_entrance/cave_normal.png","normal"); // TextureNormalCaveEntrance1
    LoadTextureImage("../../data/cave_entrance/aerial_rocks_04_diff_4k.jpg",""); // TextureImageCaveEntrance2
    LoadTextureImage("../../data/cave_entrance/aerial_rocks_04_nor_gl_4k.png","normal"); // TextureNormalCaveEntrance2
    LoadTextureImage("../../data/rabbit/texture.jpg",""); // TextureImageRabbit
    LoadTextureImage("../../data/bear/bear_co.jpg",""); // TextureImageBear
    LoadTextureImage("../../data/lettering/Mat_Paper_baseColor.png",""); // TextureImageLettering
    LoadTextureImage("../../data/lettering/FINAL1.png",""); // TextureImageEnding1
    LoadTextureImage("../../data/lettering/FINAL2.png",""); // TextureImageEnding2
    LoadTextureImage("../../data/lettering/FINAL3.png",""); // TextureImageEnding3
    LoadTextureImage("../../data/lettering/FINAL4.png",""); // TextureImageEnding4
    LoadTextureImage("../../data/lettering/FINAL5.png",""); // TextureImageEnding5
    LoadTextureImage("../../data/lettering/FINAL6.png",""); // TextureImageEnding6
    LoadTextureImage("../../data/lettering/FINAL7.png",""); // TextureImageEnding7
    LoadTextureImage("../../data/lettering/FINAL8.png",""); // TextureImageEnding8
    LoadTextureImage("../../data/lettering/FINAL9.png",""); // TextureImageEnding9


    // Construímos a representação de objetos geométricos através de malhas de triângulos
    ComputeNormals(&cavemodel);
    BuildTrianglesAndAddToVirtualScene(&cavemodel);

    ComputeNormals(&campfiremodel);
    BuildTrianglesAndAddToVirtualScene(&campfiremodel);

    ComputeNormals(&greekmodel);
    BuildTrianglesAndAddToVirtualScene(&greekmodel);

    ComputeNormals(&prisionermodel);
    BuildTrianglesAndAddToVirtualScene(&prisionermodel);

    ComputeNormals(&grutamodel);
    BuildTrianglesAndAddToVirtualScene(&grutamodel);

    ComputeNormals(&laddermodel);
    BuildTrianglesAndAddToVirtualScene(&laddermodel);

    ComputeNormals(&titlemodel);
    BuildTrianglesAndAddToVirtualScene(&titlemodel);

    ComputeNormals(&greek2model);
    BuildTrianglesAndAddToVirtualScene(&greek2model);

    ComputeNormals(&caveEntrancemodel);
    BuildTrianglesAndAddToVirtualScene(&caveEntrancemodel);

    ComputeNormals(&mountainmodel);
    BuildTrianglesAndAddToVirtualScene(&mountainmodel);

    ComputeNormals(&rabbitmodel);
    BuildTrianglesAndAddToVirtualScene(&rabbitmodel);

    ComputeNormals(&bearmodel);
    BuildTrianglesAndAddToVirtualScene(&bearmodel);

    ComputeNormals(&letteringmodel);
    BuildTrianglesAndAddToVirtualScene(&letteringmodel);


    if ( argc > 1 )
    {
        ObjModel model(argv[1]);
        BuildTrianglesAndAddToVirtualScene(&model);
    }

    // Inicializamos o código para renderização de texto.
    TextRendering_Init();

    // Habilitamos o Z-buffer. Veja slides 104-116 do documento Aula_09_Projecoes.pdf.
    glEnable(GL_DEPTH_TEST);

    // Habilitamos o Backface Culling. Veja slides 8-13 do documento Aula_02_Fundamentos_Matematicos.pdf, slides 23-34 do documento Aula_13_Clipping_and_Culling.pdf e slides 112-123 do documento Aula_14_Laboratorio_3_Revisao.pdf.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    prev_time = (float)glfwGetTime();

    // Ficamos em um loop infinito, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window))
    {
        // Aqui executamos as operações de renderização

        // Definimos a cor do "fundo" do framebuffer como branco.  Tal cor é
        // definida como coeficientes RGBA: Red, Green, Blue, Alpha; isto é:
        // Vermelho, Verde, Azul, Alpha (valor de transparência).
        // Conversaremos sobre sistemas de cores nas aulas de Modelos de Iluminação.
        //
        //           R     G     B     A
        glClearColor(0.5294117647f,0.80784313725f,0.92156862745f,0.0f);  // cor do céu

        // "Pintamos" todos os pixels do framebuffer com a cor definida acima,
        // e também resetamos todos os pixels do Z-buffer (depth buffer).
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Pedimos para a GPU utilizar o programa de GPU criado acima (contendo
        // os shaders de vértice e fragmentos).
        glUseProgram(g_GpuProgramID);

        // Atualiza delta de tempo
        current_time = (float)glfwGetTime();
        delta_t = current_time - prev_time;
        prev_time = current_time;

        // Computamos a posição da câmera utilizando coordenadas esféricas.  As
        // variáveis g_CameraDistance, g_CameraPhi, e g_CameraTheta são
        // controladas pelo mouse do usuário. Veja as funções CursorPosCallback()
        // e ScrollCallback().
        float r;
        float x;
        float y;
        float z;

        // Abaixo definimos as varáveis que efetivamente definem a câmera virtual.
        // Veja slides 195-227 e 229-234 do documento Aula_08_Sistemas_de_Coordenadas.pdf.

        camera_up_vector   = glm::vec4(0.0f,1.0f,0.0f,0.0f); // Vetor "up" fixado para apontar para o "céu" (eito Y global)

        if(!free_camera || g_ShowEnding){

            r = g_CameraDistance;
            x = r*cos(g_CameraPhi)*sin(g_CameraTheta);
            y = r*sin(g_CameraPhi);
            z = r*cos(g_CameraPhi)*cos(g_CameraTheta);

            if(g_InitialScreen || g_FirstLettering || g_ShowEnding)
            {

                g_CameraPhi = 0.0f;
                g_CameraTheta = M_PI_2;

                camera_position_c  = glm::vec4(-14.0f + 9.0f*cos(g_CameraPhi)*sin(g_CameraTheta),
                                            1.0f + y,
                                            15.5f + z,
                                            1.0f);
                camera_lookat_l    = glm::vec4(-14.0f,1.0f+2.7f,15.5f,1.0f);
            }
            else{
                camera_position_c  = glm::vec4(x + g_TorsoPositionX, y + (g_TorsoPositionY + 4.0f), z + g_TorsoPositionZ, 1.0f); // Ponto "c", centro da câmera
                camera_lookat_l    = glm::vec4(g_TorsoPositionX, g_TorsoPositionY + 2.7f, g_TorsoPositionZ, 1.0f); // Ponto "l", para onde a câmera (look-at) estará sempre olhando
            }
            camera_view_vector = camera_lookat_l - camera_position_c; // Vetor "view", sentido para onde a câmera está virada
        }
        else{
            x = r*cos(g_CameraPhi)*sin(g_CameraTheta);
            y = r*sin(g_CameraPhi);
            z = r*cos(g_CameraPhi)*cos(g_CameraTheta);

            camera_view_vector = glm::vec4(x, y, z, 0.0f);
            camera_view_vector = -camera_view_vector;

            w = -camera_view_vector;
            u = crossproduct(camera_up_vector,w);

            // Normalizamos os vetores u e w
            w = w / norm(w);
            u = u / norm(u);
        }

        glm::mat4 view = Matrix_Camera_View(camera_position_c, camera_view_vector, camera_up_vector);

        // Agora computamos a matriz de Projeção.
        glm::mat4 projection;

        // Note que, no sistema de coordenadas da câmera, os planos near e far
        // estão no sentido negativo! Veja slides 176-204 do documento Aula_09_Projecoes.pdf.
        float nearplane = -0.1f;  // Posição do "near plane"
        float farplane  = -200.0f; // Posição do "far plane"


        // Projeção Perspectiva.
        // Para definição do field of view (FOV), veja slides 205-215 do documento Aula_09_Projecoes.pdf.
        float field_of_view = 3.141592 / 3.0f;
        projection = Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);

        // Enviamos as matrizes "view" e "projection" para a placa de vídeo
        // (GPU). Veja o arquivo "shader_vertex.glsl", onde estas são
        // efetivamente aplicadas em todos os pontos.
        glUniformMatrix4fv(g_view_uniform       , 1 , GL_FALSE , glm::value_ptr(view));
        glUniformMatrix4fv(g_projection_uniform , 1 , GL_FALSE , glm::value_ptr(projection));

        //----------------------------- DECLARACOES ---------------------------------------

        float translate_x;
        float translate_y;
        float translate_z;
        float scale;
        float speed_models;

        glm::vec4 p1;
        glm::vec4 p2;
        glm::vec4 p3;

        glm::vec4 ponto_atual;

        glm::mat4 model;


        //----------------------------- JOGADOR ------------------------------------------

        if(!g_InitialScreen && !g_FirstLettering && !g_ShowEnding)
        {
            if(g_OutCave == true && ( (movement_bear_part >= 1.0f && g_ShowBear <= OutCaveNoWeaponChatID) || g_PlayerKilled ) ){
                if(g_PlayerKilled)
                    speed_models = 0.4f;
                else
                    speed_models = 0.2f;

                // Declaração dos pontos que definirão a curva criada
                p1 = glm::vec4(g_TorsoPositionX, g_TorsoPositionY, g_TorsoPositionZ, 1.0f);
                p2 = glm::vec4(g_TorsoPositionX + 0.0f, g_TorsoPositionY + 90.0f, g_TorsoPositionZ - 90.0f, 1.0f);
                p3 = glm::vec4(g_TorsoPositionX - 1.0f, g_TorsoPositionY + 100.0f, g_TorsoPositionZ - 100.0f, 1.0f);

                // A curva vai de um intervalo [0,1]
                if( (movement_bear_part >= 1.0f || g_PlayerKilled) && movement_player_part >= 0 && movement_player_part <= 1){
                    movement_player_part = movement_player_part + delta_t * speed_models;
                    ponto_atual = bezier_curve_two_degree(p1, p2, p3, movement_player_part);

                // Se a curva acabou apenas deixar o coelho na posicao final
                }else if(movement_bear_part < 1.0f){
                    ponto_atual = p1;
                }else if(movement_bear_part >= 1.0f && movement_player_part >= 1.0f){
                    ponto_atual = p3;
                }

                // Aplicação das transformacoes geométricas de acordo com o ponto retornado da funcao de bezier
                model = Matrix_Translate(ponto_atual.x, ponto_atual.y, ponto_atual.z);
            }else{
                model = Matrix_Translate(g_TorsoPositionX, g_TorsoPositionY, g_TorsoPositionZ);
            }

            model = model * Matrix_Rotate_Y(free_camera == true ? (M_PI/2 + g_AngleY_torso) : (camera_view_vector.z <= 0 ? M_PI/2 + g_AngleY_torso : M_PI/2 - g_AngleY_torso))
                          * Matrix_Rotate_X(-M_PI_2)
                          * Matrix_Rotate_Z(-M_PI_2)
                          * Matrix_Scale(0.02f,0.02f,0.02f);

            if(g_ShowOutCaveNoWeaponChat && movement_player_part > 0.5f){
                g_ShowOutCaveNoWeaponChat = false;
                g_ShowChat = false;
                g_ShowEnding = true;
                g_Ending = 3;
            }
            else if(g_ShowOutCaveWeaponChat && movement_player_part > 1.0f){
                g_ShowOutCaveWeaponChat = false;
                g_ShowChat = false;
                g_ShowEnding = true;
                g_Ending = 5;
            }
            else if(movement_player_part < 1.0f){
                AddPlayer(model);
            }

            MovePlayer();
        }

        if(!g_OutCave || g_InitialScreen || g_ShowEnding)
        {
            //----------------------------- CAVERNA ------------------------------------------

            AddCave(); //model definida dentro da função

            //----------------------------- ESCADA ------------------------------------------

            AddLadder(Matrix_Rotate_Y(-M_PI/8)
                * Matrix_Translate(-11.0f,0.4f,-0.9f)
                * Matrix_Scale(0.02f,0.02f,0.02f));

            //----------------------------- TÍTULO ------------------------------------------

            //Mostra o título apenas se o jogo estiver na tela inicial
            if(g_InitialScreen)
            {
                AddTitle(Matrix_Rotate_Y(M_PI/2)
                    * Matrix_Translate(-15.5f,4.5f,-15.0f)
                    * Matrix_Scale(0.02f,0.02f,0.02f)
                    * Matrix_Rotate_X(M_PI/16));
            }

            //----------------------------- LETTERING / FINAIS ------------------------------------------

            //Mostra o lettering ou os finais
            if(g_FirstLettering || g_ShowEnding)
            {
                AddLettering(Matrix_Translate(-7.0f,1.55f,15.5f)
                             * Matrix_Rotate_Z(-(M_PI+1)/16),g_Ending);
            }

            //----------------------------- PRISIONEIRO ------------------------------------------

            if(g_FreePrisioner <= PrisionerSecondChatOption1ID)
            {
                AddPrisioner(Matrix_Translate(2.5f,0.1f,20.0f)
                * Matrix_Scale(0.017f,0.017f,0.017f)
                * Matrix_Rotate_X(-M_PI/16),true,false);

                AddPrisioner(Matrix_Translate(5.0f,3.0f,20.0f)
                * Matrix_Scale(0.017f,0.017f,0.017f),false,true);

                AddPrisioner(Matrix_Translate(10.0f,3.0f,20.0f)
                * Matrix_Scale(0.017f,0.017f,0.017f),false, true);
            }
            else{
                AddPrisioner(Matrix_Translate(5.0f,3.0f,20.0f)
                * Matrix_Scale(0.017f,0.017f,0.017f),true,true);

                AddPrisioner(Matrix_Translate(10.0f,3.0f,20.0f)
                * Matrix_Scale(0.017f,0.017f,0.017f),false, true);
            }

            //----------------------------- SEGUNDO GREGO ------------------------------------------

            if(!g_PericlesKilled)
                AddGreek2(Matrix_Scale(0.045f,0.045f,0.045f), window);
            else
                AddGreek2(Matrix_Translate(0.0f,0.25f,0.0f) * Matrix_Scale(0.045f,0.045f,0.045f) * Matrix_Rotate_X(M_PI_2), window);

            //----------------------------- FOGUEIRA ------------------------------------------

            AddCampfire(Matrix_Translate(-14.0f,0.0f,15.5f)
                            * Matrix_Rotate_Y(M_PI_2));

            //----------------------------- FÍSICA DAS PAREDES DA CAVERNA ------------------------------------------

            AddCavePhysics(); //model definida dentro da função

        }
        else{

            //----------------------------- ENTRADA DA CAVERNA ------------------------------------------

            AddCaveEntrance(Matrix_Translate(-50.0f,4.0f,420.0f)
                            * Matrix_Scale(10.0f,10.0f,10.0f)
                                * Matrix_Rotate_Y(M_PI + M_PI/4));

            //------------------------------- COELHO ------------------------------------------

            translate_x = -58.0f;
            translate_y = 0.75f;
            translate_z = 395.0f;
            scale = 0.5f;
            speed_models = 0.2f;

            // Declaração dos pontos que definirão a curva criada
            p1 = glm::vec4(translate_x, translate_y, translate_z, 1.0f);
            p2 = glm::vec4(translate_x + 10.0f, translate_y, translate_z + 10.0f, 1.0f);
            p3 = glm::vec4(out_caveX, out_caveY, out_caveZ - 2.0f, 1.0f);

            if(g_ShowBunny <= OutCaveNoWeaponChatID || g_ShowBunny <= OutCaveWeaponChatID){

                // A curva vai de um intervalo [0,1]
                if(movement_rabbit_part >= 0 && movement_rabbit_part <= 1){
                    movement_rabbit_part = movement_rabbit_part + delta_t * speed_models;
                    ponto_atual = bezier_curve_two_degree(p1, p2, p3, movement_rabbit_part);

                // Se a curva acabou apenas deixar o coelho na posicao final
                }else{
                    ponto_atual = p3;
                }
            }
            else{
                ponto_atual = p1;
            }

            // Aplicação das transformacoes geométricas de acordo com o ponto retornado da funcao de bezier
            model = Matrix_Translate(ponto_atual.x, ponto_atual.y, ponto_atual.z)
                            * Matrix_Scale(scale, scale, scale)
                            * Matrix_Rotate_Y(M_PI/2);

            AddRabbit(model);

            //----------------------------- URSO ------------------------------------------

            translate_x = -58.0f;
            translate_y = 0.75f;
            translate_z = 470.0f;
            scale = 2.0f;
            speed_models = 0.4f;

            // Declaração dos pontos que definirão a curva criada
            p1 = glm::vec4(translate_x, translate_y, translate_z, 1.0f);
            p2 = glm::vec4(translate_x - 2.0f, translate_y + 0.5f, translate_z + 25.0f, 1.0f);
            p3 = glm::vec4(translate_x - 3.0f, translate_y, translate_z - 55.0f, 1.0f);

            // A curva vai de um intervalo [0,1]
            if(movement_rabbit_part >= 1.0f && movement_bear_part >= 0 && movement_bear_part <= 1 && g_ShowBear <= OutCaveNoWeaponChatID){
                movement_bear_part = movement_bear_part + delta_t * speed_models;
                ponto_atual = bezier_curve_two_degree(p1, p2, p3, movement_bear_part);

            // Se a curva acabou apenas deixar o coelho na posicao final
            }else if(movement_rabbit_part < 1.0f || g_ShowBear > OutCaveNoWeaponChatID){
                ponto_atual = p1;
            }else if(movement_rabbit_part >= 1.0f && movement_bear_part >= 1.0f){
                ponto_atual = p3;
            }

            // Aplicação das transformacoes geométricas de acordo com o ponto retornado da funcao de bezier
            model = Matrix_Translate(ponto_atual.x, ponto_atual.y, ponto_atual.z)
                        * Matrix_Scale(scale, scale, scale)
                        * Matrix_Rotate_Y(M_PI);

            AddBear(model);

            //----------------------------- MONTANHAS ------------------------------------------

            //Configuração das montanhas:
            //   a
            //  ┌ ┐
            //c     d
            //  └ ┘
            //   b


            // Adiciona montanha "┌"
            AddMountain(Matrix_Translate(0.0f,6.0f,150.0f + 300.0f)
                        * Matrix_Scale(150.0f,150.0f,150.0f)
                            * Matrix_Rotate_Y(M_PI));

            // Adiciona montanha "┐"
            AddMountain(Matrix_Translate(-119.0f,6.0f,150.0f + 300.0f)
                        * Matrix_Scale(150.0f,150.0f,150.0f)
                            * Matrix_Rotate_Y(M_PI));

            /*// Adiciona montanha "a"
            AddMountain(Matrix_Translate(-68.0f,6.0f,68.0f + 150.0f + 300.0f)
                        * Matrix_Scale(150.0f,150.0f,150.0f)
                            * Matrix_Rotate_Y(M_PI));*/

            // Adiciona montanha "└"
            AddMountain(Matrix_Translate(0.0f,6.0f,-136.0f + 150.0f + 300.0f)
                        * Matrix_Scale(150.0f,150.0f,150.0f)
                            * Matrix_Rotate_Y(-M_PI_2));

            /*// Adiciona montanha "c"
            AddMountain(Matrix_Translate(68.0f,6.0f,-68.0f + 150.0f + 300.0f)
                        * Matrix_Scale(150.0f,150.0f,150.0f)
                            * Matrix_Rotate_Y(-M_PI_2));*/

            // Adiciona montanha "┘"
            AddMountain(Matrix_Translate(-119.0f,6.0f,-119.0f + 150.0f + 300.0f)
                        * Matrix_Scale(150.0f,150.0f,150.0f)
                            * Matrix_Rotate_Y(0.0f));

            /*// Adiciona montanha "b"
            AddMountain(Matrix_Translate(-68.0f,6.0f,-136.0f*1.5f + 150.0f + 300.0f)
                        * Matrix_Scale(150.0f,150.0f,150.0f));

            // Adiciona montanha "d"
            AddMountain(Matrix_Translate(-136.0f * 1.5f,6.0f,-68.0f + 150.0f + 300.0f)
                        * Matrix_Scale(150.0f,150.0f,150.0f)
                            * Matrix_Rotate_Y(M_PI));*/

        }

        //----------------------------- TEXTOS ------------------------------------------

        // Imprimimos na tela informação sobre o número de quadros renderizados
        // por segundo (frames per second).
        //TextRendering_ShowFramesPerSecond(window);

        //Texto tela inicial
        if(g_InitialScreen)
        {
            if(g_InitialScreen_FirstTime)
            {
                TextRendering_ShowInitialScreenText(window, "APERTE [ENTER] PARA COMECAR", 1.5f);
            }
            else{
                TextRendering_ShowInitialScreenText(window, "APERTE [ENTER] PARA VOLTAR AO JOGO", 1.5f);
            }
        }

        //Texto primeira fala
        if(g_ShowFirstChat)
        {
            TextRendering_ShowChatCharacters(window, "(Jogador) => Ahn... o que? Eu estou... livre.", 1.5f, -0.8);
            TextRendering_ShowInferiorCommand(window, "[ENTER] Passar", 1.5f);
        }

        //Texto conversa com prisioneiro
        if( (g_PrisionerRockCollision || g_ShowPrisionerSecondChat) && !g_ShowEnding)
        {
            if(!g_isSecondChat)
            {
                if(!g_ShowPrisionerFirstChat)
                {
                    TextRendering_ShowChatCharacters(window, "[SPACE] Conversar", 1.5f, -0.8);
                    TextRendering_ShowInferiorCommand(window, "Prisioneiro", 1.5f);
                }
                else if(PrisionerFirstChatOption == 0){
                    TextRendering_ShowChatCharacters(window, PrisionerFirstChat[PrisionerFirstChatID], 1.5f, -0.8);
                    if(PrisionerFirstChatID != PrisionerFirstChatMAX - 1)
                    {
                        TextRendering_ShowInferiorCommand(window, "[ENTER] Passar", 1.5f);
                    }
                }
                else if(PrisionerFirstChatOption == 1){
                    TextRendering_ShowChatCharacters(window, PrisionerFirstChatOption1, 1.5f, -0.8);
                    TextRendering_ShowInferiorCommand(window, "[ENTER] Passar", 1.5f);
                }
                else if(PrisionerFirstChatOption == 2){
                    TextRendering_ShowChatCharacters(window, PrisionerFirstChatOption2[PrisionerFirstChatOption2ID], 1.5f, -0.8);
                    TextRendering_ShowInferiorCommand(window, "[ENTER] Passar", 1.5f);
                }
            }
            else{
                if(!g_ShowPrisionerSecondChat)
                {
                    TextRendering_ShowChatCharacters(window, "[SPACE] Conversar", 1.5f, -0.8);
                    TextRendering_ShowInferiorCommand(window, "Prometheus", 1.5f);
                }
                else if(PrisionerSecondChatOption == 0){
                    TextRendering_ShowChatCharacters(window, PrisionerSecondChat[PrisionerSecondChatID], 1.5f, -0.8);
                    if(PrisionerSecondChatID != PrisionerSecondChatMAX - 1)
                    {
                        TextRendering_ShowInferiorCommand(window, "[ENTER] Passar", 1.5f);
                    }
                }
                else if(PrisionerSecondChatOption == 1){
                    TextRendering_ShowChatCharacters(window, PrisionerSecondChatOption1[PrisionerSecondChatOption1ID], 1.5f, -0.8);
                    TextRendering_ShowInferiorCommand(window, "[ENTER] Passar", 1.5f);
                }
                else if(PrisionerSecondChatOption == 2){
                    TextRendering_ShowChatCharacters(window, PrisionerSecondChatOption2[PrisionerSecondChatOption2ID], 1.5f, -0.8);
                    TextRendering_ShowInferiorCommand(window, "[ENTER] Passar", 1.5f);
                }
            }
        }

        //Texto conversa com Pericles
        if(g_Greek2Collision && !g_ShowEnding)
        {
            if(!g_ShowPericlesFirstChat)
            {
                if(!g_PericlesKilled)
                {
                    TextRendering_ShowChatCharacters(window, "[SPACE] Conversar", 1.5f, -0.8);
                    TextRendering_ShowInferiorCommand(window, "Pericles", 1.5f);
                }
            }
            else if(PericlesFirstChatOption == 0){
                TextRendering_ShowChatCharacters(window, PericlesFirstChat[PericlesFirstChatID], 1.5f, -0.8);
                if(PericlesFirstChatID != PericlesFirstChatMAX - 1)
                {
                    TextRendering_ShowInferiorCommand(window, "[ENTER] Passar", 1.5f);
                }
                else{
                    float currentTimeDecision = (float) glfwGetTime();

                    if(startTimeDecision == 0.0)
                    {
                        startTimeDecision = currentTimeDecision;
                    }
                    else if( currentTimeDecision - startTimeDecision >= timeDecision )
                    {
                        PericlesFirstChatOption = 4;
                    }
                }
            }
            else if(PericlesFirstChatOption == 1){
                TextRendering_ShowChatCharacters(window, PericlesFirstChatOption1, 1.5f, -0.8);
                TextRendering_ShowInferiorCommand(window, "[ENTER] Passar", 1.5f);
            }
            else if(PericlesFirstChatOption == 2){
                TextRendering_ShowChatCharacters(window, PericlesFirstChatOption2[PericlesFirstChatOption2ID], 1.5f, -0.8);
                TextRendering_ShowInferiorCommand(window, "[ENTER] Passar", 1.5f);
            }
            else if(PericlesFirstChatOption == 3){
                TextRendering_ShowChatCharacters(window, PericlesFirstChatOption3[PericlesFirstChatOption3ID], 1.5f, -0.8);
                TextRendering_ShowInferiorCommand(window, "[ENTER] Passar", 1.5f);
            }
            else if(PericlesFirstChatOption == 4){
                TextRendering_ShowChatCharacters(window, PericlesFirstChatOption4, 1.5f, -0.8);
                TextRendering_ShowInferiorCommand(window, "[ENTER] Passar", 1.5f);
            }
        }

        if(g_LadderCollision && !g_ShowEnding)
        {
            TextRendering_ShowChatCharacters(window, "[SPACE] Sair da Caverna", 1.5f, -0.8);
        }

        if(g_OutCave && !g_ShowEnding){
            //TextRendering_ShowChatCharacters(window, "Movimente a camera para vizualizar a cutscene", 1.5f, 0.8);
            if(g_Weapon)
            {
                if(OutCaveWeaponChatOption == 0){
                    TextRendering_ShowChatCharacters(window, OutCaveWeaponChat[OutCaveWeaponChatID], 1.5f, -0.8);
                    if(OutCaveWeaponChatID != OutCaveWeaponChatMAX - 1)
                    {
                        TextRendering_ShowInferiorCommand(window, "[ENTER] Passar", 1.5f);
                    }
                }
                else if(OutCaveWeaponChatOption == 1){
                    TextRendering_ShowChatCharacters(window, OutCaveWeaponChatOption1, 1.5f, -0.8);
                    TextRendering_ShowInferiorCommand(window, "[ENTER] Passar", 1.5f);
                }
                else if(OutCaveWeaponChatOption == 2){
                    TextRendering_ShowChatCharacters(window, OutCaveWeaponChatOption2, 1.5f, -0.8);
                    TextRendering_ShowInferiorCommand(window, "[ENTER] Passar", 1.5f);
                }
                else if(OutCaveWeaponChatOption == 3){
                    TextRendering_ShowChatCharacters(window, OutCaveWeaponChatOption3, 1.5f, -0.8);
                    TextRendering_ShowInferiorCommand(window, "[ENTER] Passar", 1.5f);
                }
            }
            else{
                if(OutCaveNoWeaponChatID < OutCaveNoWeaponChatMAX)
                    TextRendering_ShowChatCharacters(window, OutCaveNoWeaponChat[OutCaveNoWeaponChatID], 1.5f, -0.8);
                TextRendering_ShowInferiorCommand(window, "[ENTER] Passar", 1.5f);
            }
        }


        // O framebuffer onde OpenGL executa as operações de renderização não
        // é o mesmo que está sendo mostrado para o usuário, caso contrário
        // seria possível ver artefatos conhecidos como "screen tearing". A
        // chamada abaixo faz a troca dos buffers, mostrando para o usuário
        // tudo que foi renderizado pelas funções acima.
        // Veja o link: https://en.wikipedia.org/w/index.php?title=Multiple_buffering&oldid=793452829#Double_buffering_in_computer_graphics
        glfwSwapBuffers(window);

        // Verificamos com o sistema operacional se houve alguma interação do
        // usuário (teclado, mouse, ...). Caso positivo, as funções de callback
        // definidas anteriormente usando glfwSet*Callback() serão chamadas
        // pela biblioteca GLFW.
        glfwPollEvents();
    }

    // Finalizamos o uso dos recursos do sistema operacional
    glfwTerminate();

    // Fim do programa
    return 0;
}

// Função que carrega uma imagem para ser utilizada como textura
void LoadTextureImage(const char* filename, const char* type)
{
    printf("Carregando imagem \"%s\"... ", filename);

    // Primeiro fazemos a leitura da imagem do disco
    stbi_set_flip_vertically_on_load(true);
    int width;
    int height;
    int channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 3);

    if ( data == NULL )
    {
        fprintf(stderr, "ERROR: Cannot open image file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }

    printf("OK (%dx%d).\n", width, height);

    // Agora criamos objetos na GPU com OpenGL para armazenar a textura
    GLuint texture_id;
    GLuint sampler_id;
    glGenTextures(1, &texture_id);
    glGenSamplers(1, &sampler_id);

    // Veja slides 95-96 do documento Aula_20_Mapeamento_de_Texturas.pdf
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Parâmetros de amostragem da textura.
    glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Agora enviamos a imagem lida do disco para a GPU
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    GLuint textureunit = g_NumLoadedTextures;
    glActiveTexture(GL_TEXTURE0 + textureunit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    if(type == "normal")
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else if(type == "alpha")
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, data);
    }
    else{
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindSampler(textureunit, sampler_id);

    stbi_image_free(data);

    g_NumLoadedTextures += 1;
}

// Função que desenha um objeto armazenado em g_VirtualScene. Veja definição
// dos objetos na função BuildTrianglesAndAddToVirtualScene().
void DrawVirtualObject(const char* object_name)
{
    // "Ligamos" o VAO. Informamos que queremos utilizar os atributos de
    // vértices apontados pelo VAO criado pela função BuildTrianglesAndAddToVirtualScene(). Veja
    // comentários detalhados dentro da definição de BuildTrianglesAndAddToVirtualScene().
    glBindVertexArray(g_VirtualScene[object_name].vertex_array_object_id);

    // Setamos as variáveis "bbox_min" e "bbox_max" do fragment shader
    // com os parâmetros da axis-aligned bounding box (AABB) do modelo.
    glm::vec3 bbox_min = g_VirtualScene[object_name].bbox_min;
    glm::vec3 bbox_max = g_VirtualScene[object_name].bbox_max;
    glUniform4f(g_bbox_min_uniform, bbox_min.x, bbox_min.y, bbox_min.z, 1.0f);
    glUniform4f(g_bbox_max_uniform, bbox_max.x, bbox_max.y, bbox_max.z, 1.0f);

    // Pedimos para a GPU rasterizar os vértices dos eixos XYZ
    // apontados pelo VAO como linhas. Veja a definição de
    // g_VirtualScene[""] dentro da função BuildTrianglesAndAddToVirtualScene(), e veja
    // a documentação da função glDrawElements() em
    // http://docs.gl/gl3/glDrawElements.
    glDrawElements(
        g_VirtualScene[object_name].rendering_mode,
        g_VirtualScene[object_name].num_indices,
        GL_UNSIGNED_INT,
        (void*)(g_VirtualScene[object_name].first_index * sizeof(GLuint))
    );

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);
}

// Função que carrega os shaders de vértices e de fragmentos que serão
// utilizados para renderização. Veja slides 180-200 do documento Aula_03_Rendering_Pipeline_Grafico.pdf.
//
void LoadShadersFromFiles()
{
    // Note que o caminho para os arquivos "shader_vertex.glsl" e
    // "shader_fragment.glsl" estão fixados, sendo que assumimos a existência
    // da seguinte estrutura no sistema de arquivos:
    //
    //    + FCG_Lab_01/
    //    |
    //    +--+ bin/
    //    |  |
    //    |  +--+ Release/  (ou Debug/ ou Linux/)
    //    |     |
    //    |     o-- main.exe
    //    |
    //    +--+ src/
    //       |
    //       o-- shader_vertex.glsl
    //       |
    //       o-- shader_fragment.glsl
    //
    GLuint vertex_shader_id = LoadShader_Vertex("../../src/shader_vertex.glsl");
    GLuint fragment_shader_id = LoadShader_Fragment("../../src/shader_fragment.glsl");

    // Deletamos o programa de GPU anterior, caso ele exista.
    if ( g_GpuProgramID != 0 )
        glDeleteProgram(g_GpuProgramID);

    // Criamos um programa de GPU utilizando os shaders carregados acima.
    g_GpuProgramID = CreateGpuProgram(vertex_shader_id, fragment_shader_id);

    // Buscamos o endereço das variáveis definidas dentro do Vertex Shader.
    // Utilizaremos estas variáveis para enviar dados para a placa de vídeo
    // (GPU)! Veja arquivo "shader_vertex.glsl" e "shader_fragment.glsl".
    g_model_uniform      = glGetUniformLocation(g_GpuProgramID, "model"); // Variável da matriz "model"
    g_view_uniform       = glGetUniformLocation(g_GpuProgramID, "view"); // Variável da matriz "view" em shader_vertex.glsl
    g_projection_uniform = glGetUniformLocation(g_GpuProgramID, "projection"); // Variável da matriz "projection" em shader_vertex.glsl
    g_object_id_uniform  = glGetUniformLocation(g_GpuProgramID, "object_id"); // Variável "object_id" em shader_fragment.glsl
    g_bbox_min_uniform   = glGetUniformLocation(g_GpuProgramID, "bbox_min");
    g_bbox_max_uniform   = glGetUniformLocation(g_GpuProgramID, "bbox_max");

    // Variáveis em "shader_fragment.glsl" para acesso das imagens de textura
    glUseProgram(g_GpuProgramID);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageCampfire"), 0);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageFire"), 1);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageCaveWalls"), 2);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageCaveFloor"), 3);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageGruta"), 4);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageGreekBody"), 5);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageGreekHead"), 6);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageTitle"), 7);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImagePrisioner"), 8);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImagePrisionerEyes"), 9);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImagePrisionerRock"), 10);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageLadder"), 11);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageGreek2"), 12);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageMountain"), 13);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureNormalMountain"), 14);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageCaveEntrance1"), 15);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureNormalCaveEntrance1"), 16);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageCaveEntrance2"), 17);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureNormalCaveEntrance2"),18);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageRabbit"), 19);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageBear"), 20);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageLettering"), 21);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageEnding1"), 22);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageEnding2"), 23);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageEnding3"), 24);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageEnding4"), 25);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageEnding5"), 26);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageEnding6"), 27);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageEnding7"), 28);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageEnding8"), 29);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImageEnding9"), 30);
    glUseProgram(0);
}

// Função que pega a matriz M e guarda a mesma no topo da pilha
void PushMatrix(glm::mat4 M)
{
    g_MatrixStack.push(M);
}

// Função que remove a matriz atualmente no topo da pilha e armazena a mesma na variável M
void PopMatrix(glm::mat4& M)
{
    if ( g_MatrixStack.empty() )
    {
        M = Matrix_Identity();
    }
    else
    {
        M = g_MatrixStack.top();
        g_MatrixStack.pop();
    }
}

// Função que computa as normais de um ObjModel, caso elas não tenham sido
// especificadas dentro do arquivo ".obj"
void ComputeNormals(ObjModel* model)
{
    if ( !model->attrib.normals.empty() )
        return;

    // Primeiro computamos as normais para todos os TRIÂNGULOS.
    // Segundo, computamos as normais dos VÉRTICES através do método proposto
    // por Gouraud, onde a normal de cada vértice vai ser a média das normais de
    // todas as faces que compartilham este vértice.

    size_t num_vertices = model->attrib.vertices.size() / 3;

    std::vector<int> num_triangles_per_vertex(num_vertices, 0);
    std::vector<glm::vec4> vertex_normals(num_vertices, glm::vec4(0.0f,0.0f,0.0f,0.0f));

    for (size_t shape = 0; shape < model->shapes.size(); ++shape)
    {
        size_t num_triangles = model->shapes[shape].mesh.num_face_vertices.size();

        for (size_t triangle = 0; triangle < num_triangles; ++triangle)
        {
            assert(model->shapes[shape].mesh.num_face_vertices[triangle] == 3);

            glm::vec4  vertices[3];
            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = model->shapes[shape].mesh.indices[3*triangle + vertex];
                const float vx = model->attrib.vertices[3*idx.vertex_index + 0];
                const float vy = model->attrib.vertices[3*idx.vertex_index + 1];
                const float vz = model->attrib.vertices[3*idx.vertex_index + 2];
                vertices[vertex] = glm::vec4(vx,vy,vz,1.0);
            }

            const glm::vec4  a = vertices[0];
            const glm::vec4  b = vertices[1];
            const glm::vec4  c = vertices[2];

            const glm::vec4  n = crossproduct(b-a,c-a);

            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = model->shapes[shape].mesh.indices[3*triangle + vertex];
                num_triangles_per_vertex[idx.vertex_index] += 1;
                vertex_normals[idx.vertex_index] += n;
                model->shapes[shape].mesh.indices[3*triangle + vertex].normal_index = idx.vertex_index;
            }
        }
    }

    model->attrib.normals.resize( 3*num_vertices );

    for (size_t i = 0; i < vertex_normals.size(); ++i)
    {
        glm::vec4 n = vertex_normals[i] / (float)num_triangles_per_vertex[i];
        n /= norm(n);
        model->attrib.normals[3*i + 0] = n.x;
        model->attrib.normals[3*i + 1] = n.y;
        model->attrib.normals[3*i + 2] = n.z;
    }
}

// Constrói triângulos para futura renderização a partir de um ObjModel.
void BuildTrianglesAndAddToVirtualScene(ObjModel* model)
{
    GLuint vertex_array_object_id;
    glGenVertexArrays(1, &vertex_array_object_id);
    glBindVertexArray(vertex_array_object_id);

    std::vector<GLuint> indices;
    std::vector<float>  model_coefficients;
    std::vector<float>  normal_coefficients;
    std::vector<float>  texture_coefficients;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    std::vector<glm::vec3> crossEdges;
    std::vector<float> isOutCave;

    for (size_t shape = 0; shape < model->shapes.size(); ++shape)
    {
        size_t first_index = indices.size();
        size_t num_triangles = model->shapes[shape].mesh.num_face_vertices.size();

        const float minval = std::numeric_limits<float>::min();
        const float maxval = std::numeric_limits<float>::max();

        glm::vec3 bbox_min = glm::vec3(maxval,maxval,maxval);
        glm::vec3 bbox_max = glm::vec3(minval,minval,minval);

        for (size_t triangle = 0; triangle < num_triangles; ++triangle)
        {
            assert(model->shapes[shape].mesh.num_face_vertices[triangle] == 3);

            for (size_t vertex = 0; vertex < 3; ++vertex)
            {

                tinyobj::index_t idx = model->shapes[shape].mesh.indices[3*triangle + vertex];

                indices.push_back(first_index + 3*triangle + vertex);

                const float vx = model->attrib.vertices[3*idx.vertex_index + 0];
                const float vy = model->attrib.vertices[3*idx.vertex_index + 1];
                const float vz = model->attrib.vertices[3*idx.vertex_index + 2];

                model_coefficients.push_back( vx ); // X
                model_coefficients.push_back( vy ); // Y
                model_coefficients.push_back( vz ); // Z
                model_coefficients.push_back( 1.0f ); // W

                bbox_min.x = std::min(bbox_min.x, vx);
                bbox_min.y = std::min(bbox_min.y, vy);
                bbox_min.z = std::min(bbox_min.z, vz);
                bbox_max.x = std::max(bbox_max.x, vx);
                bbox_max.y = std::max(bbox_max.y, vy);
                bbox_max.z = std::max(bbox_max.z, vz);

                // Inspecionando o código da tinyobjloader, o aluno Bernardo
                // Sulzbach (2017/1) apontou que a maneira correta de testar se
                // existem normais e coordenadas de textura no ObjModel é
                // comparando se o índice retornado é -1. Fazemos isso abaixo.

                if ( idx.normal_index != -1 )
                {
                    const float nx = model->attrib.normals[3*idx.normal_index + 0];
                    const float ny = model->attrib.normals[3*idx.normal_index + 1];
                    const float nz = model->attrib.normals[3*idx.normal_index + 2];
                    normal_coefficients.push_back( nx ); // X
                    normal_coefficients.push_back( ny ); // Y
                    normal_coefficients.push_back( nz ); // Z
                    normal_coefficients.push_back( 0.0f ); // W
                }

                if ( idx.texcoord_index != -1 )
                {
                    const float u = model->attrib.texcoords[2*idx.texcoord_index + 0];
                    const float v = model->attrib.texcoords[2*idx.texcoord_index + 1];
                    texture_coefficients.push_back( u );
                    texture_coefficients.push_back( v );
                }

                //Inidicador se o personagem saiu da caverna ou não
                if(g_OutCave)
                {
                    isOutCave.push_back(1.0f);
                }
                else{
                    isOutCave.push_back(0.0f);
                }

            }


            //------------------------ Calculo usado para normal mapping -------------

            tinyobj::index_t idxAux = model->shapes[shape].mesh.indices[3*triangle + 0];
            glm::vec3 v0 = glm::vec3(model->attrib.vertices[3*idxAux.vertex_index + 0],
                                     model->attrib.vertices[3*idxAux.vertex_index + 1],
                                     model->attrib.vertices[3*idxAux.vertex_index + 2]);

            glm::vec2 uv0 = glm::vec2(model->attrib.texcoords[2*idxAux.texcoord_index + 0],
                                      model->attrib.texcoords[2*idxAux.texcoord_index + 1]);

            idxAux = model->shapes[shape].mesh.indices[3*triangle + 1];
            glm::vec3 v1 = glm::vec3(model->attrib.vertices[3*idxAux.vertex_index + 0],
                                     model->attrib.vertices[3*idxAux.vertex_index + 1],
                                     model->attrib.vertices[3*idxAux.vertex_index + 2]);

            glm::vec2 uv1 = glm::vec2(model->attrib.texcoords[2*idxAux.texcoord_index + 0],
                                      model->attrib.texcoords[2*idxAux.texcoord_index + 1]);

            idxAux = model->shapes[shape].mesh.indices[3*triangle + 2];
            glm::vec3 v2 = glm::vec3(model->attrib.vertices[3*idxAux.vertex_index + 0],
                                     model->attrib.vertices[3*idxAux.vertex_index + 1],
                                     model->attrib.vertices[3*idxAux.vertex_index + 2]);

            glm::vec2 uv2 = glm::vec2(model->attrib.texcoords[2*idxAux.texcoord_index + 0],
                                      model->attrib.texcoords[2*idxAux.texcoord_index + 1]);

            // Edges of the triangle : position delta
            glm::vec3 deltaPos1 = v1-v0;
            glm::vec3 deltaPos2 = v2-v0;
            glm::vec3 crossDelta = cross(deltaPos1,deltaPos2);
            crossEdges.push_back(crossDelta);
            crossEdges.push_back(crossDelta);
            crossEdges.push_back(crossDelta);

            // UV delta
            glm::vec2 deltaUV1 = uv1-uv0;
            glm::vec2 deltaUV2 = uv2-uv0;

            float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
            glm::vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
            glm::vec3 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;

            // Set the same tangent for all three vertices of the triangle.
            // They will be merged later, in vboindexer.cpp
            tangents.push_back(tangent);
            tangents.push_back(tangent);
            tangents.push_back(tangent);

            // Same thing for bitangents
            bitangents.push_back(bitangent);
            bitangents.push_back(bitangent);
            bitangents.push_back(bitangent);
        }

        size_t last_index = indices.size() - 1;

        SceneObject theobject;
        theobject.name           = model->shapes[shape].name;
        theobject.first_index    = first_index; // Primeiro índice
        theobject.num_indices    = last_index - first_index + 1; // Número de indices
        theobject.rendering_mode = GL_TRIANGLES;       // Índices correspondem ao tipo de rasterização GL_TRIANGLES.
        theobject.vertex_array_object_id = vertex_array_object_id;

        theobject.bbox_min = bbox_min;
        theobject.bbox_max = bbox_max;

        g_VirtualScene[model->shapes[shape].name] = theobject;
    }

    GLuint VBO_model_coefficients_id;
    glGenBuffers(1, &VBO_model_coefficients_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, model_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, model_coefficients.size() * sizeof(float), model_coefficients.data());
    GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
    GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if ( !normal_coefficients.empty() )
    {
        GLuint VBO_normal_coefficients_id;
        glGenBuffers(1, &VBO_normal_coefficients_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_coefficients_id);
        glBufferData(GL_ARRAY_BUFFER, normal_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, normal_coefficients.size() * sizeof(float), normal_coefficients.data());
        location = 1; // "(location = 1)" em "shader_vertex.glsl"
        number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if ( !texture_coefficients.empty() )
    {
        GLuint VBO_texture_coefficients_id;
        glGenBuffers(1, &VBO_texture_coefficients_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_texture_coefficients_id);
        glBufferData(GL_ARRAY_BUFFER, texture_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, texture_coefficients.size() * sizeof(float), texture_coefficients.data());
        location = 2; // "(location = 1)" em "shader_vertex.glsl"
        number_of_dimensions = 2; // vec2 em "shader_vertex.glsl"
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

        GLuint VBO_tangents_id;
        glGenBuffers(1, &VBO_tangents_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_tangents_id);
        glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, tangents.size() * sizeof(glm::vec3), tangents.data());
        location = 3; // "(location = 1)" em "shader_vertex.glsl"
        number_of_dimensions = 3; // vec2 em "shader_vertex.glsl"
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        GLuint VBO_bitangents_id;
        glGenBuffers(1, &VBO_bitangents_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_bitangents_id);
        glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, bitangents.size() * sizeof(glm::vec3), bitangents.data());
        location = 4; // "(location = 1)" em "shader_vertex.glsl"
        number_of_dimensions = 3; // vec2 em "shader_vertex.glsl"
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        GLuint VBO_crossEdges_id;
        glGenBuffers(1, &VBO_crossEdges_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_crossEdges_id);
        glBufferData(GL_ARRAY_BUFFER, crossEdges.size() * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, crossEdges.size() * sizeof(glm::vec3), crossEdges.data());
        location = 5; // "(location = 1)" em "shader_vertex.glsl"
        number_of_dimensions = 3; // vec2 em "shader_vertex.glsl"
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        GLuint VBO_isOutCave_id;
        glGenBuffers(1, &VBO_isOutCave_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_isOutCave_id);
        glBufferData(GL_ARRAY_BUFFER, isOutCave.size() * sizeof(float), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, isOutCave.size() * sizeof(float), isOutCave.data());
        location = 6; // "(location = 1)" em "shader_vertex.glsl"
        number_of_dimensions = 1; // vec2 em "shader_vertex.glsl"
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint indices_id;
    glGenBuffers(1, &indices_id);

    // "Ligamos" o buffer. Note que o tipo agora é GL_ELEMENT_ARRAY_BUFFER.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // XXX Errado!
    //

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);
}

// Carrega um Vertex Shader de um arquivo GLSL. Veja definição de LoadShader() abaixo.
GLuint LoadShader_Vertex(const char* filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // será aplicado nos vértices.
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

    // Carregamos e compilamos o shader
    LoadShader(filename, vertex_shader_id);

    // Retorna o ID gerado acima
    return vertex_shader_id;
}

// Carrega um Fragment Shader de um arquivo GLSL . Veja definição de LoadShader() abaixo.
GLuint LoadShader_Fragment(const char* filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // será aplicado nos fragmentos.
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Carregamos e compilamos o shader
    LoadShader(filename, fragment_shader_id);

    // Retorna o ID gerado acima
    return fragment_shader_id;
}

// Função auxilar, utilizada pelas duas funções acima. Carrega código de GPU de
// um arquivo GLSL e faz sua compilação.
void LoadShader(const char* filename, GLuint shader_id)
{
    // Lemos o arquivo de texto indicado pela variável "filename"
    // e colocamos seu conteúdo em memória, apontado pela variável
    // "shader_string".
    std::ifstream file;
    try {
        file.exceptions(std::ifstream::failbit);
        file.open(filename);
    } catch ( std::exception& e ) {
        fprintf(stderr, "ERROR: Cannot open file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }
    std::stringstream shader;
    shader << file.rdbuf();
    std::string str = shader.str();
    const GLchar* shader_string = str.c_str();
    const GLint   shader_string_length = static_cast<GLint>( str.length() );

    // Define o código do shader GLSL, contido na string "shader_string"
    glShaderSource(shader_id, 1, &shader_string, &shader_string_length);

    // Compila o código do shader GLSL (em tempo de execução)
    glCompileShader(shader_id);

    // Verificamos se ocorreu algum erro ou "warning" durante a compilação
    GLint compiled_ok;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok);

    GLint log_length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

    // Alocamos memória para guardar o log de compilação.
    // A chamada "new" em C++ é equivalente ao "malloc()" do C.
    GLchar* log = new GLchar[log_length];
    glGetShaderInfoLog(shader_id, log_length, &log_length, log);

    // Imprime no terminal qualquer erro ou "warning" de compilação
    if ( log_length != 0 )
    {
        std::string  output;

        if ( !compiled_ok )
        {
            output += "ERROR: OpenGL compilation of \"";
            output += filename;
            output += "\" failed.\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }
        else
        {
            output += "WARNING: OpenGL compilation of \"";
            output += filename;
            output += "\".\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }

        fprintf(stderr, "%s", output.c_str());
    }

    // A chamada "delete" em C++ é equivalente ao "free()" do C
    delete [] log;
}

// Esta função cria um programa de GPU, o qual contém obrigatoriamente um
// Vertex Shader e um Fragment Shader.
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id)
{
    // Criamos um identificador (ID) para este programa de GPU
    GLuint program_id = glCreateProgram();

    // Definição dos dois shaders GLSL que devem ser executados pelo programa
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    // Linkagem dos shaders acima ao programa
    glLinkProgram(program_id);

    // Verificamos se ocorreu algum erro durante a linkagem
    GLint linked_ok = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &linked_ok);

    // Imprime no terminal qualquer erro de linkagem
    if ( linked_ok == GL_FALSE )
    {
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        // Alocamos memória para guardar o log de compilação.
        // A chamada "new" em C++ é equivalente ao "malloc()" do C.
        GLchar* log = new GLchar[log_length];

        glGetProgramInfoLog(program_id, log_length, &log_length, log);

        std::string output;

        output += "ERROR: OpenGL linking of program failed.\n";
        output += "== Start of link log\n";
        output += log;
        output += "\n== End of link log\n";

        // A chamada "delete" em C++ é equivalente ao "free()" do C
        delete [] log;

        fprintf(stderr, "%s", output.c_str());
    }

    // Os "Shader Objects" podem ser marcados para deleção após serem linkados
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    // Retornamos o ID gerado acima
    return program_id;
}

// Definição da função que será chamada sempre que a janela do sistema
// operacional for redimensionada, por consequência alterando o tamanho do
// "framebuffer" (região de memória onde são armazenados os pixels da imagem).
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Indicamos que queremos renderizar em toda região do framebuffer. A
    // função "glViewport" define o mapeamento das "normalized device
    // coordinates" (NDC) para "pixel coordinates".  Essa é a operação de
    // "Screen Mapping" ou "Viewport Mapping" vista em aula ({+ViewportMapping2+}).
    glViewport(0, 0, width, height);

    // Atualizamos também a razão que define a proporção da janela (largura /
    // altura), a qual será utilizada na definição das matrizes de projeção,
    // tal que não ocorra distorções durante o processo de "Screen Mapping"
    // acima, quando NDC é mapeado para coordenadas de pixels. Veja slides 205-215 do documento Aula_09_Projecoes.pdf.
    //
    // O cast para float é necessário pois números inteiros são arredondados ao
    // serem divididos!
    g_ScreenRatio = (float)width / height;
}

// Variáveis globais que armazenam a última posição do cursor do mouse, para
// que possamos calcular quanto que o mouse se movimentou entre dois instantes
// de tempo. Utilizadas no callback CursorPosCallback() abaixo.
double g_LastCursorPosX, g_LastCursorPosY;

// Função callback chamada sempre que o usuário aperta algum dos botões do mouse
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // Se o usuário pressionou o botão esquerdo do mouse, guardamos a
        // posição atual do cursor nas variáveis g_LastCursorPosX e
        // g_LastCursorPosY.  Também, setamos a variável
        // g_LeftMouseButtonPressed como true, para saber que o usuário está
        // com o botão esquerdo pressionado.
        glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        g_LeftMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        // Quando o usuário soltar o botão esquerdo do mouse, atualizamos a
        // variável abaixo para false.
        g_LeftMouseButtonPressed = false;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        // Se o usuário pressionou o botão esquerdo do mouse, guardamos a
        // posição atual do cursor nas variáveis g_LastCursorPosX e
        // g_LastCursorPosY.  Também, setamos a variável
        // g_RightMouseButtonPressed como true, para saber que o usuário está
        // com o botão esquerdo pressionado.
        glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        g_RightMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        // Quando o usuário soltar o botão esquerdo do mouse, atualizamos a
        // variável abaixo para false.
        g_RightMouseButtonPressed = false;
    }
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
    {
        // Se o usuário pressionou o botão esquerdo do mouse, guardamos a
        // posição atual do cursor nas variáveis g_LastCursorPosX e
        // g_LastCursorPosY.  Também, setamos a variável
        // g_MiddleMouseButtonPressed como true, para saber que o usuário está
        // com o botão esquerdo pressionado.
        glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        g_MiddleMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
    {
        // Quando o usuário soltar o botão esquerdo do mouse, atualizamos a
        // variável abaixo para false.
        g_MiddleMouseButtonPressed = false;
    }
}

// Função callback chamada sempre que o usuário movimentar o cursor do mouse em
// cima da janela OpenGL.
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    // Abaixo executamos o seguinte: caso o botão esquerdo do mouse esteja
    // pressionado, computamos quanto que o mouse se movimento desde o último
    // instante de tempo, e usamos esta movimentação para atualizar os
    // parâmetros que definem a posição da câmera dentro da cena virtual.
    // Assim, temos que o usuário consegue controlar a câmera.

    if (g_LeftMouseButtonPressed && !g_InitialScreen && !g_FirstLettering && !g_ShowEnding)
    {
        // Deslocamento do cursor do mouse em x e y de coordenadas de tela!
        //float dx = xpos - g_LastCursorPosX;
        float dy = ypos - g_LastCursorPosY;

        // Atualizamos parâmetros da câmera com os deslocamentos
        if(free_camera == true){
            float dx = xpos - g_LastCursorPosX;
            g_CameraTheta -= 0.01f*dx;
        }

        g_CameraPhi   += 0.01f*dy;

        // Em coordenadas esféricas, o ângulo phi deve ficar entre -pi/2 e +pi/2.
        float phimax = 3.141592f/2 - 0.05;
        float phimin = -phimax;

        if (g_CameraPhi > phimax)
            g_CameraPhi = phimax;

        if (g_CameraPhi < phimin)
            g_CameraPhi = phimin;

        // Atualizamos as variáveis globais para armazenar a posição atual do
        // cursor como sendo a última posição conhecida do cursor.
        g_LastCursorPosX = xpos;
        g_LastCursorPosY = ypos;
    }
}

// Função callback chamada sempre que o usuário movimenta a "rodinha" do mouse.
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(1 == 0){
        g_CameraDistance -= 0.2f*yoffset;

        const float verysmallnumber = 1.0f;
        const float superiorLimit = 8.0f;

        if (g_CameraDistance < verysmallnumber)
            g_CameraDistance = verysmallnumber;
        if(g_CameraDistance > superiorLimit)
            g_CameraDistance = superiorLimit;
    }
}

// Definição da função que será chamada sempre que o usuário pressionar alguma
// tecla do teclado. Veja http://www.glfw.org/docs/latest/input_guide.html#input_key
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    // ====================
    // Não modifique este loop! Ele é utilizando para correção automatizada dos
    // laboratórios. Deve ser sempre o primeiro comando desta função KeyCallback().
    for (int i = 0; i < 10; ++i)
        if (key == GLFW_KEY_0 + i && action == GLFW_PRESS && mod == GLFW_MOD_SHIFT)
            std::exit(100 + i);
    // ====================

    // Se o usuário pressionar a tecla ESC, fechamos a janela.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // Jogo começa quando sai da tela inicial
    if(!g_InitialScreen && !g_FirstLettering && !g_ShowEnding)
    {

        // Se o usuário apertar a tecla W, movimentamos a câmera para frente.
        if ( (key == GLFW_KEY_W || key == GLFW_KEY_UP) && action == GLFW_PRESS && !g_ShowChat )
        {
            g_WKeyPressed = true;
        }
        else if( (key == GLFW_KEY_W || key == GLFW_KEY_UP) && action == GLFW_RELEASE){
            g_WKeyPressed = false;
        }

        // Se o usuário apertar a tecla S, movimentamos a câmera para trás.
        if ( (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) && action == GLFW_PRESS && !g_ShowChat )
        {
            g_SKeyPressed = true;
        }
        else if( (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) && action == GLFW_RELEASE){
            g_SKeyPressed = false;
        }

        // Se o usuário apertar a tecla A, movimentamos a câmera para a esquerda.
        if ( (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && action == GLFW_PRESS )
        {
            g_AKeyPressed = true;
        }
        else if( (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && action == GLFW_RELEASE){
            g_AKeyPressed = false;
        }

        // Se o usuário apertar a tecla D, movimentamos a câmera para a direita.
        if ( (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && action == GLFW_PRESS)
        {
            g_DKeyPressed = true;
        }
        else if( (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && action == GLFW_RELEASE){
            g_DKeyPressed = false;
        }

        /*// Se o usuário apertar a tecla ",", movimentamos a câmera para cima.
        if (key == GLFW_KEY_COMMA && action == GLFW_PRESS && !g_ShowChat)
        {
            g_COMMAKeyPressed = true;
        }
        else if(key == GLFW_KEY_COMMA && action == GLFW_RELEASE){
            g_COMMAKeyPressed = false;
        }

        // Se o usuário apertar a tecla ".", movimentamos a câmera para baixo.
        if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS && !g_ShowChat)
        {
            g_PERIODKeyPressed = true;
        }
        else if(key == GLFW_KEY_PERIOD && action == GLFW_RELEASE){
            g_PERIODKeyPressed = false;
        }*/

        // Se o usuário apertar o shift esquerdo o personagem anda mais rápido.
        if (key == GLFW_KEY_LEFT_SHIFT  && action == GLFW_PRESS && !g_ShowChat)
        {
            speed = fast_speed;
        }
        else if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE){
            speed = normal_speed;
        }

        // Se o usuário apertar space
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        {
            if(g_PrisionerRockCollision)
            {
                if(!g_isSecondChat)
                    g_ShowPrisionerFirstChat = true;
                else
                    g_ShowPrisionerSecondChat = true;
                g_ShowChat = true;
            }

            if(g_Greek2Collision)
            {
                g_ShowPericlesFirstChat = true;
                g_ShowChat = true;
            }

            if(g_LadderCollision)
            {
                g_OutCave = true;

                free_camera = true;
                g_LadderCollision = false;

                g_CameraTheta = M_PI + M_PI/4 + M_PI/8;
                g_CameraPhi = 0.0f;
                g_TorsoPositionX = out_caveX;
                g_TorsoPositionY = out_caveY;
                g_TorsoPositionZ = out_caveZ;

                camera_position_c  = glm::vec4(g_TorsoPositionX - 8.0f, g_TorsoPositionY + 4.0f, g_TorsoPositionZ - 8.0f, 1.0f);

                if(g_Weapon)
                {
                    g_ShowOutCaveWeaponChat = true;
                }
                else{
                    g_ShowOutCaveNoWeaponChat = true;
                }
            }
        }

        // Se o usuário quiser usar a câmera livre
        if(key == GLFW_KEY_C && action == GLFW_PRESS){
            //free_camera = free_camera == true ? false : (g_OutCave == true ? true : false);
        }

        if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
        {
            if(g_ShowFirstChat)
            {
                g_ShowFirstChat = false;
                g_ShowChat = false;
            }
            else if(g_ShowPrisionerFirstChat)
            {
                if(PrisionerFirstChatID < PrisionerFirstChatMAX - 1)
                {
                    PrisionerFirstChatID++;
                }

                if(PrisionerFirstChatID == PrisionerFirstChatMAX - 1)
                {
                    if(PrisionerFirstChatOption == 0)
                    {
                        PrisionerFirstChatChoose = true;
                    }
                    else if(PrisionerFirstChatOption == 1)
                    {
                        g_ShowPrisionerFirstChat = false;
                        g_ShowChat = false;
                        g_ShowEnding = true;
                        g_Ending = 1;
                    }
                    else if(PrisionerFirstChatOption == 2)
                    {
                        if(PrisionerFirstChatOption2ID < PrisionerFirstChatOption2MAX - 1)
                        {
                            PrisionerFirstChatOption2ID++;
                        }
                        else{
                            g_ShowPrisionerFirstChat = false;
                            g_ShowChat = false;
                        }
                    }
                }
            }
            else if(g_ShowPericlesFirstChat)
            {
                if(PericlesFirstChatID < PericlesFirstChatMAX - 1)
                {
                    PericlesFirstChatID++;
                }

                if(PericlesFirstChatID == PericlesFirstChatMAX - 1)
                {
                    if(PericlesFirstChatOption == 0)
                    {
                        PericlesFirstChatChoose = true;
                    }
                    else if(PericlesFirstChatOption == 1)
                    {
                        g_ShowPericlesFirstChat = false;
                        g_ShowChat = false;
                        g_PericlesKilled = true;
                        g_Greek2Collision = false;
                        g_Weapon = true;
                    }
                    else if(PericlesFirstChatOption == 2)
                    {
                        if(PericlesFirstChatOption2ID < PericlesFirstChatOption2MAX - 1)
                        {
                            PericlesFirstChatOption2ID++;
                        }
                        else{
                            g_ShowPericlesFirstChat = false;
                            g_ShowChat = false;
                            g_Weapon = true;
                        }
                    }
                    else if(PericlesFirstChatOption == 3)
                    {
                        if(PericlesFirstChatOption3ID < PericlesFirstChatOption3MAX - 1)
                        {
                            PericlesFirstChatOption3ID++;
                        }
                        else{
                            g_ShowPericlesFirstChat = false;
                            g_ShowChat = false;
                        }
                    }
                    else if (PericlesFirstChatOption == 4)
                    {
                        g_ShowPericlesFirstChat = false;
                        g_ShowChat = false;
                        g_ShowEnding = true;
                        g_Ending = 2;
                    }
                }
            }
            else if(g_ShowOutCaveNoWeaponChat)
            {
                if(OutCaveNoWeaponChatID <= OutCaveNoWeaponChatMAX)
                {
                    OutCaveNoWeaponChatID++;
                }
            }
            else if(g_ShowOutCaveWeaponChat)
            {
                if(OutCaveWeaponChatID < OutCaveWeaponChatMAX - 1)
                {
                    OutCaveWeaponChatID++;
                }

                if(OutCaveWeaponChatID == OutCaveWeaponChatMAX - 1)
                {
                    if(OutCaveWeaponChatOption == 0)
                    {
                        OutCaveWeaponChatChoose = true;
                    }
                    else if(OutCaveWeaponChatOption == 1)
                    {
                        g_ShowOutCaveWeaponChat = false;
                        g_ShowChat = false;
                        g_ShowEnding = true;
                        g_Ending = 4;
                    }
                    else if(OutCaveWeaponChatOption == 2)
                    {
                        g_PlayerKilled = true;
                    }
                    else if(OutCaveWeaponChatOption == 3)
                    {
                        g_ShowOutCaveWeaponChat = false;
                        g_ShowChat = false;
                        g_OutCave = false;
                        free_camera = false;
                        g_TorsoPositionX = -7.0f;
                        g_TorsoPositionY = 0.0f;
                        g_TorsoPositionZ = -5.0f;
                        g_isSecondChat = true;
                    }
                }
            }
            else if(g_ShowPrisionerSecondChat)
            {
                if(PrisionerSecondChatID < PrisionerSecondChatMAX - 1)
                {
                    PrisionerSecondChatID++;
                }

                if(PrisionerSecondChatID == PrisionerSecondChatMAX - 1)
                {
                    if(PrisionerSecondChatOption == 0)
                    {
                        PrisionerSecondChatChoose = true;
                    }
                    else if(PrisionerSecondChatOption == 1)
                    {
                        if(PrisionerSecondChatOption1ID < PrisionerSecondChatOption1MAX - 1)
                        {
                            PrisionerSecondChatOption1ID++;
                        }
                        else{
                            g_ShowPrisionerSecondChat = false;
                            g_ShowChat = false;
                            g_ShowEnding = true;
                            if(g_PericlesKilled)
                                g_Ending = 6;
                            else
                                g_Ending = 7;
                        }
                    }
                    else if(PrisionerSecondChatOption == 2)
                    {
                        if(PrisionerSecondChatOption2ID < PrisionerSecondChatOption2MAX - 1)
                        {
                            PrisionerSecondChatOption2ID++;
                        }
                        else{
                            g_ShowPrisionerSecondChat = false;
                            g_ShowChat = false;
                            g_ShowEnding = true;
                            if(g_PericlesKilled)
                                g_Ending = 8;
                            else
                                g_Ending = 9;
                        }
                    }
                }
            }

        }

        if(key == GLFW_KEY_1 && action == GLFW_PRESS)
        {
            if(PrisionerFirstChatChoose)
            {
                PrisionerFirstChatOption = 1;
                PrisionerFirstChatChoose = false;
            }

            if(PericlesFirstChatChoose)
            {
                PericlesFirstChatOption = 1;
                PericlesFirstChatChoose = false;
            }

            if(OutCaveWeaponChatChoose)
            {
                OutCaveWeaponChatOption = 1;
                OutCaveWeaponChatChoose = false;
            }

            if(PrisionerSecondChatChoose)
            {
                PrisionerSecondChatOption = 1;
                PrisionerSecondChatChoose = false;
            }
        }

        if(key == GLFW_KEY_2 && action == GLFW_PRESS)
        {
            if(PrisionerFirstChatChoose)
            {
                PrisionerFirstChatOption = 2;
                PrisionerFirstChatChoose = false;
            }

            if(PericlesFirstChatChoose)
            {
                PericlesFirstChatOption = 2;
                PericlesFirstChatChoose = false;
            }

            if(OutCaveWeaponChatChoose)
            {
                OutCaveWeaponChatOption = 2;
                OutCaveWeaponChatChoose = false;
            }

            if(PrisionerSecondChatChoose)
            {
                PrisionerSecondChatOption = 2;
                PrisionerSecondChatChoose = false;
            }
        }

        if(key == GLFW_KEY_3 && action == GLFW_PRESS)
        {
            if(PericlesFirstChatChoose)
            {
                PericlesFirstChatOption = 3;
                PericlesFirstChatChoose = false;
            }

            if(OutCaveWeaponChatChoose)
            {
                OutCaveWeaponChatOption = 3;
                OutCaveWeaponChatChoose = false;
            }
        }

    }
    else{
        // Se o usuário apertar a tecla enter na tela inicial, começa o jogo.
        if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
        {
            if(g_InitialScreen)
            {
                g_InitialScreen = false;
                g_InitialScreen_FirstTime = false;

                g_FirstLettering = true;
            }
            else if(g_FirstLettering)
            {
                g_FirstLettering = false;
                g_ShowFirstChat = true;
                g_ShowChat = true;

                g_CameraPhi = 0.0f;
                g_CameraTheta = M_PI/4;

            }
        }
    }

    // Se o usuário apertar a tecla R, recarregamos os shaders dos arquivos "shader_fragment.glsl" e "shader_vertex.glsl".
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        LoadShadersFromFiles();
        fprintf(stdout,"Shaders recarregados!\n");
        fflush(stdout);
    }
}

// Definimos o callback para impressão de erros da GLFW no terminal
void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "ERROR: GLFW: %s\n", description);
}

// Esta função recebe um vértice com coordenadas de modelo p_model e passa o
// mesmo por todos os sistemas de coordenadas armazenados nas matrizes model,
// view, e projection; e escreve na tela as matrizes e pontos resultantes
// dessas transformações.
void TextRendering_ShowModelViewProjection(
    GLFWwindow* window,
    glm::mat4 projection,
    glm::mat4 view,
    glm::mat4 model,
    glm::vec4 p_model
)
{
    if ( !g_ShowInfoText )
        return;

    glm::vec4 p_world = model*p_model;
    glm::vec4 p_camera = view*p_world;
    glm::vec4 p_clip = projection*p_camera;
    glm::vec4 p_ndc = p_clip / p_clip.w;

    float pad = TextRendering_LineHeight(window);

    TextRendering_PrintString(window, " Model matrix             Model     In World Coords.", -1.0f, 1.0f-pad, 1.0f);
    TextRendering_PrintMatrixVectorProduct(window, model, p_model, -1.0f, 1.0f-2*pad, 1.0f);

    TextRendering_PrintString(window, "                                        |  ", -1.0f, 1.0f-6*pad, 1.0f);
    TextRendering_PrintString(window, "                            .-----------'  ", -1.0f, 1.0f-7*pad, 1.0f);
    TextRendering_PrintString(window, "                            V              ", -1.0f, 1.0f-8*pad, 1.0f);

    TextRendering_PrintString(window, " View matrix              World     In Camera Coords.", -1.0f, 1.0f-9*pad, 1.0f);
    TextRendering_PrintMatrixVectorProduct(window, view, p_world, -1.0f, 1.0f-10*pad, 1.0f);

    TextRendering_PrintString(window, "                                        |  ", -1.0f, 1.0f-14*pad, 1.0f);
    TextRendering_PrintString(window, "                            .-----------'  ", -1.0f, 1.0f-15*pad, 1.0f);
    TextRendering_PrintString(window, "                            V              ", -1.0f, 1.0f-16*pad, 1.0f);

    TextRendering_PrintString(window, " Projection matrix        Camera                    In NDC", -1.0f, 1.0f-17*pad, 1.0f);
    TextRendering_PrintMatrixVectorProductDivW(window, projection, p_camera, -1.0f, 1.0f-18*pad, 1.0f);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glm::vec2 a = glm::vec2(-1, -1);
    glm::vec2 b = glm::vec2(+1, +1);
    glm::vec2 p = glm::vec2( 0,  0);
    glm::vec2 q = glm::vec2(width, height);

    glm::mat4 viewport_mapping = Matrix(
        (q.x - p.x)/(b.x-a.x), 0.0f, 0.0f, (b.x*p.x - a.x*q.x)/(b.x-a.x),
        0.0f, (q.y - p.y)/(b.y-a.y), 0.0f, (b.y*p.y - a.y*q.y)/(b.y-a.y),
        0.0f , 0.0f , 1.0f , 0.0f ,
        0.0f , 0.0f , 0.0f , 1.0f
    );

    TextRendering_PrintString(window, "                                                       |  ", -1.0f, 1.0f-22*pad, 1.0f);
    TextRendering_PrintString(window, "                            .--------------------------'  ", -1.0f, 1.0f-23*pad, 1.0f);
    TextRendering_PrintString(window, "                            V                           ", -1.0f, 1.0f-24*pad, 1.0f);

    TextRendering_PrintString(window, " Viewport matrix           NDC      In Pixel Coords.", -1.0f, 1.0f-25*pad, 1.0f);
    TextRendering_PrintMatrixVectorProductMoreDigits(window, viewport_mapping, p_ndc, -1.0f, 1.0f-26*pad, 1.0f);
}

// Escrevemos na tela o número de quadros renderizados por segundo (frames per
// second).
void TextRendering_ShowFramesPerSecond(GLFWwindow* window)
{
    if ( !g_ShowInfoText )
        return;

    // Variáveis estáticas (static) mantém seus valores entre chamadas
    // subsequentes da função!
    static float old_seconds = (float)glfwGetTime();
    static int   ellapsed_frames = 0;
    static char  buffer[20] = "?? fps";
    static int   numchars = 7;

    ellapsed_frames += 1;

    // Recuperamos o número de segundos que passou desde a execução do programa
    float seconds = (float)glfwGetTime();

    // Número de segundos desde o último cálculo do fps
    float ellapsed_seconds = seconds - old_seconds;

    if ( ellapsed_seconds > 1.0f )
    {
        numchars = snprintf(buffer, 20, "%.2f fps", ellapsed_frames / ellapsed_seconds);
        //numchars = snprintf(buffer, 20, "%f,%f", g_TorsoPositionX, g_TorsoPositionZ);

        old_seconds = seconds;
        ellapsed_frames = 0;
    }

    float lineheight = TextRendering_LineHeight(window);
    float charwidth = TextRendering_CharWidth(window);

    TextRendering_PrintString(window, buffer, 1.0f-(numchars + 1)*charwidth, 1.0f-lineheight, 1.0f);
}

// Função para debugging: imprime no terminal todas informações de um modelo
// geométrico carregado de um arquivo ".obj".
// Veja: https://github.com/syoyo/tinyobjloader/blob/22883def8db9ef1f3ffb9b404318e7dd25fdbb51/loader_example.cc#L98
void PrintObjModelInfo(ObjModel* model)
{
  const tinyobj::attrib_t                & attrib    = model->attrib;
  const std::vector<tinyobj::shape_t>    & shapes    = model->shapes;
  const std::vector<tinyobj::material_t> & materials = model->materials;

  printf("# of vertices  : %d\n", (int)(attrib.vertices.size() / 3));
  printf("# of normals   : %d\n", (int)(attrib.normals.size() / 3));
  printf("# of texcoords : %d\n", (int)(attrib.texcoords.size() / 2));
  printf("# of shapes    : %d\n", (int)shapes.size());
  printf("# of materials : %d\n", (int)materials.size());

  for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
    printf("  v[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
           static_cast<const double>(attrib.vertices[3 * v + 0]),
           static_cast<const double>(attrib.vertices[3 * v + 1]),
           static_cast<const double>(attrib.vertices[3 * v + 2]));
  }

  for (size_t v = 0; v < attrib.normals.size() / 3; v++) {
    printf("  n[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
           static_cast<const double>(attrib.normals[3 * v + 0]),
           static_cast<const double>(attrib.normals[3 * v + 1]),
           static_cast<const double>(attrib.normals[3 * v + 2]));
  }

  for (size_t v = 0; v < attrib.texcoords.size() / 2; v++) {
    printf("  uv[%ld] = (%f, %f)\n", static_cast<long>(v),
           static_cast<const double>(attrib.texcoords[2 * v + 0]),
           static_cast<const double>(attrib.texcoords[2 * v + 1]));
  }

  // For each shape
  for (size_t i = 0; i < shapes.size(); i++) {
    printf("shape[%ld].name = %s\n", static_cast<long>(i),
           shapes[i].name.c_str());
    printf("Size of shape[%ld].indices: %lu\n", static_cast<long>(i),
           static_cast<unsigned long>(shapes[i].mesh.indices.size()));

    size_t index_offset = 0;

    assert(shapes[i].mesh.num_face_vertices.size() ==
           shapes[i].mesh.material_ids.size());

    printf("shape[%ld].num_faces: %lu\n", static_cast<long>(i),
           static_cast<unsigned long>(shapes[i].mesh.num_face_vertices.size()));

    // For each face
    for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
      size_t fnum = shapes[i].mesh.num_face_vertices[f];

      printf("  face[%ld].fnum = %ld\n", static_cast<long>(f),
             static_cast<unsigned long>(fnum));

      // For each vertex in the face
      for (size_t v = 0; v < fnum; v++) {
        tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
        printf("    face[%ld].v[%ld].idx = %d/%d/%d\n", static_cast<long>(f),
               static_cast<long>(v), idx.vertex_index, idx.normal_index,
               idx.texcoord_index);
      }

      printf("  face[%ld].material_id = %d\n", static_cast<long>(f),
             shapes[i].mesh.material_ids[f]);

      index_offset += fnum;
    }

    printf("shape[%ld].num_tags: %lu\n", static_cast<long>(i),
           static_cast<unsigned long>(shapes[i].mesh.tags.size()));
    for (size_t t = 0; t < shapes[i].mesh.tags.size(); t++) {
      printf("  tag[%ld] = %s ", static_cast<long>(t),
             shapes[i].mesh.tags[t].name.c_str());
      printf(" ints: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].intValues.size(); ++j) {
        printf("%ld", static_cast<long>(shapes[i].mesh.tags[t].intValues[j]));
        if (j < (shapes[i].mesh.tags[t].intValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");

      printf(" floats: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].floatValues.size(); ++j) {
        printf("%f", static_cast<const double>(
                         shapes[i].mesh.tags[t].floatValues[j]));
        if (j < (shapes[i].mesh.tags[t].floatValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");

      printf(" strings: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].stringValues.size(); ++j) {
        printf("%s", shapes[i].mesh.tags[t].stringValues[j].c_str());
        if (j < (shapes[i].mesh.tags[t].stringValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");
      printf("\n");
    }
  }

  for (size_t i = 0; i < materials.size(); i++) {
    printf("material[%ld].name = %s\n", static_cast<long>(i),
           materials[i].name.c_str());
    printf("  material.Ka = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].ambient[0]),
           static_cast<const double>(materials[i].ambient[1]),
           static_cast<const double>(materials[i].ambient[2]));
    printf("  material.Kd = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].diffuse[0]),
           static_cast<const double>(materials[i].diffuse[1]),
           static_cast<const double>(materials[i].diffuse[2]));
    printf("  material.Ks = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].specular[0]),
           static_cast<const double>(materials[i].specular[1]),
           static_cast<const double>(materials[i].specular[2]));
    printf("  material.Tr = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].transmittance[0]),
           static_cast<const double>(materials[i].transmittance[1]),
           static_cast<const double>(materials[i].transmittance[2]));
    printf("  material.Ke = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].emission[0]),
           static_cast<const double>(materials[i].emission[1]),
           static_cast<const double>(materials[i].emission[2]));
    printf("  material.Ns = %f\n",
           static_cast<const double>(materials[i].shininess));
    printf("  material.Ni = %f\n", static_cast<const double>(materials[i].ior));
    printf("  material.dissolve = %f\n",
           static_cast<const double>(materials[i].dissolve));
    printf("  material.illum = %d\n", materials[i].illum);
    printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
    printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
    printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
    printf("  material.map_Ns = %s\n",
           materials[i].specular_highlight_texname.c_str());
    printf("  material.map_bump = %s\n", materials[i].bump_texname.c_str());
    printf("  material.map_d = %s\n", materials[i].alpha_texname.c_str());
    printf("  material.disp = %s\n", materials[i].displacement_texname.c_str());
    printf("  <<PBR>>\n");
    printf("  material.Pr     = %f\n", materials[i].roughness);
    printf("  material.Pm     = %f\n", materials[i].metallic);
    printf("  material.Ps     = %f\n", materials[i].sheen);
    printf("  material.Pc     = %f\n", materials[i].clearcoat_thickness);
    printf("  material.Pcr    = %f\n", materials[i].clearcoat_thickness);
    printf("  material.aniso  = %f\n", materials[i].anisotropy);
    printf("  material.anisor = %f\n", materials[i].anisotropy_rotation);
    printf("  material.map_Ke = %s\n", materials[i].emissive_texname.c_str());
    printf("  material.map_Pr = %s\n", materials[i].roughness_texname.c_str());
    printf("  material.map_Pm = %s\n", materials[i].metallic_texname.c_str());
    printf("  material.map_Ps = %s\n", materials[i].sheen_texname.c_str());
    printf("  material.norm   = %s\n", materials[i].normal_texname.c_str());
    std::map<std::string, std::string>::const_iterator it(
        materials[i].unknown_parameter.begin());
    std::map<std::string, std::string>::const_iterator itEnd(
        materials[i].unknown_parameter.end());

    for (; it != itEnd; it++) {
      printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
    }
    printf("\n");
  }
}


// Função que desenha um cubo com arestas em preto, definido dentro da função BuildTriangles().
void DrawCube(GLint render_as_black_uniform)
{
    // Informamos para a placa de vídeo (GPU) que a variável booleana
    // "render_as_black" deve ser colocada como "false". Veja o arquivo
    // "shader_vertex.glsl".
    glUniform1i(render_as_black_uniform, false);

    // Pedimos para a GPU rasterizar os vértices do cubo apontados pelo
    // VAO como triângulos, formando as faces do cubo. Esta
    // renderização irá executar o Vertex Shader definido no arquivo
    // "shader_vertex.glsl", e o mesmo irá utilizar as matrizes
    // "model", "view" e "projection" definidas acima e já enviadas
    // para a placa de vídeo (GPU).
    //
    // Veja a definição de g_VirtualScene["cube_faces"] dentro da
    // função BuildTriangles(), e veja a documentação da função
    // glDrawElements() em http://docs.gl/gl3/glDrawElements.
    glDrawElements(
        g_VirtualScene["cube_faces"].rendering_mode, // Veja slides 182-188 do documento Aula_04_Modelagem_Geometrica_3D.pdf
        g_VirtualScene["cube_faces"].num_indices,    //
        GL_UNSIGNED_INT,
        (void*)g_VirtualScene["cube_faces"].first_index
    );

    // Pedimos para OpenGL desenhar linhas com largura de 4 pixels.
    glLineWidth(4.0f);

    // Pedimos para a GPU rasterizar os vértices dos eixos XYZ
    // apontados pelo VAO como linhas. Veja a definição de
    // g_VirtualScene["axes"] dentro da função BuildTriangles(), e veja
    // a documentação da função glDrawElements() em
    // http://docs.gl/gl3/glDrawElements.
    //
    // Importante: estes eixos serão desenhamos com a matriz "model"
    // definida acima, e portanto sofrerão as mesmas transformações
    // geométricas que o cubo. Isto é, estes eixos estarão
    // representando o sistema de coordenadas do modelo (e não o global)!


    // Informamos para a placa de vídeo (GPU) que a variável booleana
    // "render_as_black" deve ser colocada como "true". Veja o arquivo
    // "shader_vertex.glsl".
    glUniform1i(render_as_black_uniform, true);

    // Pedimos para a GPU rasterizar os vértices do cubo apontados pelo
    // VAO como linhas, formando as arestas pretas do cubo. Veja a
    // definição de g_VirtualScene["cube_edges"] dentro da função
    // BuildTriangles(), e veja a documentação da função
    // glDrawElements() em http://docs.gl/gl3/glDrawElements.
    glDrawElements(
        g_VirtualScene["cube_edges"].rendering_mode,
        g_VirtualScene["cube_edges"].num_indices,
        GL_UNSIGNED_INT,
        (void*)g_VirtualScene["cube_edges"].first_index
    );
}

GLuint BuildTriangles()
{
    // Primeiro, definimos os atributos de cada vértice.

    // A posição de cada vértice é definida por coeficientes em um sistema de
    // coordenadas local de cada modelo geométrico. Note o uso de coordenadas
    // homogêneas.  Veja as seguintes referências:
    //
    //  - slides 35-48 do documento Aula_08_Sistemas_de_Coordenadas.pdf;
    //  - slides 184-190 do documento Aula_08_Sistemas_de_Coordenadas.pdf;
    //
    // Este vetor "model_coefficients" define a GEOMETRIA (veja slides 103-110 do documento Aula_04_Modelagem_Geometrica_3D.pdf).
    //
    GLfloat model_coefficients[] = {
    // Vértices de um cubo
    //    X      Y     Z     W
        -0.5f,  0.0f,  0.5f, 1.0f, // posição do vértice 0
        -0.5f, -1.0f,  0.5f, 1.0f, // posição do vértice 1
         0.5f, -1.0f,  0.5f, 1.0f, // posição do vértice 2
         0.5f,  0.0f,  0.5f, 1.0f, // posição do vértice 3
        -0.5f,  0.0f, -0.5f, 1.0f, // posição do vértice 4
        -0.5f, -1.0f, -0.5f, 1.0f, // posição do vértice 5
         0.5f, -1.0f, -0.5f, 1.0f, // posição do vértice 6
         0.5f,  0.0f, -0.5f, 1.0f, // posição do vértice 7
    // Vértices para desenhar o eixo X
    //    X      Y     Z     W
         0.0f,  0.0f,  0.0f, 1.0f, // posição do vértice 8
         1.0f,  0.0f,  0.0f, 1.0f, // posição do vértice 9
    // Vértices para desenhar o eixo Y
    //    X      Y     Z     W
         0.0f,  0.0f,  0.0f, 1.0f, // posição do vértice 10
         0.0f,  1.0f,  0.0f, 1.0f, // posição do vértice 11
    // Vértices para desenhar o eixo Z
    //    X      Y     Z     W
         0.0f,  0.0f,  0.0f, 1.0f, // posição do vértice 12
         0.0f,  0.0f,  1.0f, 1.0f, // posição do vértice 13
    };

    // Criamos o identificador (ID) de um Vertex Buffer Object (VBO).  Um VBO é
    // um buffer de memória que irá conter os valores de um certo atributo de
    // um conjunto de vértices; por exemplo: posição, cor, normais, coordenadas
    // de textura.  Neste exemplo utilizaremos vários VBOs, um para cada tipo de atributo.
    // Agora criamos um VBO para armazenarmos um atributo: posição.
    GLuint VBO_model_coefficients_id;
    glGenBuffers(1, &VBO_model_coefficients_id);

    // Criamos o identificador (ID) de um Vertex Array Object (VAO).  Um VAO
    // contém a definição de vários atributos de um certo conjunto de vértices;
    // isto é, um VAO irá conter ponteiros para vários VBOs.
    GLuint vertex_array_object_id;
    glGenVertexArrays(1, &vertex_array_object_id);

    // "Ligamos" o VAO ("bind"). Informamos que iremos atualizar o VAO cujo ID
    // está contido na variável "vertex_array_object_id".
    glBindVertexArray(vertex_array_object_id);

    // "Ligamos" o VBO ("bind"). Informamos que o VBO cujo ID está contido na
    // variável VBO_model_coefficients_id será modificado a seguir. A
    // constante "GL_ARRAY_BUFFER" informa que esse buffer é de fato um VBO, e
    // irá conter atributos de vértices.
    glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);

    // Alocamos memória para o VBO "ligado" acima. Como queremos armazenar
    // nesse VBO todos os valores contidos no array "model_coefficients", pedimos
    // para alocar um número de bytes exatamente igual ao tamanho ("size")
    // desse array. A constante "GL_STATIC_DRAW" dá uma dica para o driver da
    // GPU sobre como utilizaremos os dados do VBO. Neste caso, estamos dizendo
    // que não pretendemos alterar tais dados (são estáticos: "STATIC"), e
    // também dizemos que tais dados serão utilizados para renderizar ou
    // desenhar ("DRAW").  Pense que:
    //
    //            glBufferData()  ==  malloc() do C  ==  new do C++.
    //
    glBufferData(GL_ARRAY_BUFFER, sizeof(model_coefficients), NULL, GL_STATIC_DRAW);

    // Finalmente, copiamos os valores do array model_coefficients para dentro do
    // VBO "ligado" acima.  Pense que:
    //
    //            glBufferSubData()  ==  memcpy() do C.
    //
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(model_coefficients), model_coefficients);

    // Precisamos então informar um índice de "local" ("location"), o qual será
    // utilizado no shader "shader_vertex.glsl" para acessar os valores
    // armazenados no VBO "ligado" acima. Também, informamos a dimensão (número de
    // coeficientes) destes atributos. Como em nosso caso são pontos em coordenadas
    // homogêneas, temos quatro coeficientes por vértice (X,Y,Z,W). Isso define
    // um tipo de dado chamado de "vec4" em "shader_vertex.glsl": um vetor com
    // quatro coeficientes. Finalmente, informamos que os dados estão em ponto
    // flutuante com 32 bits (GL_FLOAT).
    // Esta função também informa que o VBO "ligado" acima em glBindBuffer()
    // está dentro do VAO "ligado" acima por glBindVertexArray().
    // Veja https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Buffer_Object
    GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
    GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);

    // "Ativamos" os atributos. Informamos que os atributos com índice de local
    // definido acima, na variável "location", deve ser utilizado durante o
    // rendering.
    glEnableVertexAttribArray(location);

    // "Desligamos" o VBO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Agora repetimos todos os passos acima para atribuir um novo atributo a
    // cada vértice: uma cor (veja slides 109-112 do documento Aula_03_Rendering_Pipeline_Grafico.pdf e slide 111 do documento Aula_04_Modelagem_Geometrica_3D.pdf).
    // Tal cor é definida como coeficientes RGBA: Red, Green, Blue, Alpha;
    // isto é: Vermelho, Verde, Azul, Alpha (valor de transparência).
    // Conversaremos sobre sistemas de cores nas aulas de Modelos de Iluminação.
    GLfloat color_coefficients[] = {
    // Cores dos vértices do cubo
    //  R     G     B     A
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 0
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 1
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 2
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 3
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 4
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 5
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 6
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 7
    // Cores para desenhar o eixo X
        1.0f, 0.0f, 0.0f, 1.0f, // cor do vértice 8
        1.0f, 0.0f, 0.0f, 1.0f, // cor do vértice 9
    // Cores para desenhar o eixo Y
        0.0f, 1.0f, 0.0f, 1.0f, // cor do vértice 10
        0.0f, 1.0f, 0.0f, 1.0f, // cor do vértice 11
    // Cores para desenhar o eixo Z
        0.0f, 0.0f, 1.0f, 1.0f, // cor do vértice 12
        0.0f, 0.0f, 1.0f, 1.0f, // cor do vértice 13
    };
    GLuint VBO_color_coefficients_id;
    glGenBuffers(1, &VBO_color_coefficients_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_coefficients), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(color_coefficients), color_coefficients);
    location = 1; // "(location = 1)" em "shader_vertex.glsl"
    number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Vamos então definir polígonos utilizando os vértices do array
    // model_coefficients.
    //
    // Para referência sobre os modos de renderização, veja slides 182-188 do documento Aula_04_Modelagem_Geometrica_3D.pdf.
    //
    // Este vetor "indices" define a TOPOLOGIA (veja slides 103-110 do documento Aula_04_Modelagem_Geometrica_3D.pdf).
    //
    GLuint indices[] = {
    // Definimos os índices dos vértices que definem as FACES de um cubo
    // através de 12 triângulos que serão desenhados com o modo de renderização
    // GL_TRIANGLES.
        0, 1, 2, // triângulo 1
        7, 6, 5, // triângulo 2
        3, 2, 6, // triângulo 3
        4, 0, 3, // triângulo 4
        4, 5, 1, // triângulo 5
        1, 5, 6, // triângulo 6
        0, 2, 3, // triângulo 7
        7, 5, 4, // triângulo 8
        3, 6, 7, // triângulo 9
        4, 3, 7, // triângulo 10
        4, 1, 0, // triângulo 11
        1, 6, 2, // triângulo 12
    // Definimos os índices dos vértices que definem as ARESTAS de um cubo
    // através de 12 linhas que serão desenhadas com o modo de renderização
    // GL_LINES.
        0, 1, // linha 1
        1, 2, // linha 2
        2, 3, // linha 3
        3, 0, // linha 4
        0, 4, // linha 5
        4, 7, // linha 6
        7, 6, // linha 7
        6, 2, // linha 8
        6, 5, // linha 9
        5, 4, // linha 10
        5, 1, // linha 11
        7, 3, // linha 12
    // Definimos os índices dos vértices que definem as linhas dos eixos X, Y,
    // Z, que serão desenhados com o modo GL_LINES.
        8 , 9 , // linha 1
        10, 11, // linha 2
        12, 13  // linha 3
    };

    // Criamos um primeiro objeto virtual (SceneObject) que se refere às faces
    // coloridas do cubo.
    SceneObject cube_faces;
    cube_faces.name           = "Cubo (faces coloridas)";
    cube_faces.first_index    = 0; // Primeiro índice está em indices[0]
    cube_faces.num_indices    = 36;       // Último índice está em indices[35]; total de 36 índices.
    cube_faces.rendering_mode = GL_TRIANGLES; // Índices correspondem ao tipo de rasterização GL_TRIANGLES.

    // Adicionamos o objeto criado acima na nossa cena virtual (g_VirtualScene).
    g_VirtualScene["cube_faces"] = cube_faces;

    // Criamos um segundo objeto virtual (SceneObject) que se refere às arestas
    // pretas do cubo.
    SceneObject cube_edges;
    cube_edges.name           = "Cubo (arestas pretas)";
    cube_edges.first_index    = (36*sizeof(GLuint)); // Primeiro índice está em indices[36]
    cube_edges.num_indices    = 24; // Último índice está em indices[59]; total de 24 índices.
    cube_edges.rendering_mode = GL_LINES; // Índices correspondem ao tipo de rasterização GL_LINES.

    // Adicionamos o objeto criado acima na nossa cena virtual (g_VirtualScene).
    g_VirtualScene["cube_edges"] = cube_edges;

    // Criamos um terceiro objeto virtual (SceneObject) que se refere aos eixos XYZ.
    SceneObject axes;
    axes.name           = "Eixos XYZ";
    axes.first_index    = (60*sizeof(GLuint)); // Primeiro índice está em indices[60]
    axes.num_indices    = 6; // Último índice está em indices[65]; total de 6 índices.
    axes.rendering_mode = GL_LINES; // Índices correspondem ao tipo de rasterização GL_LINES.
    g_VirtualScene["axes"] = axes;

    // Criamos um buffer OpenGL para armazenar os índices acima
    GLuint indices_id;
    glGenBuffers(1, &indices_id);

    // "Ligamos" o buffer. Note que o tipo agora é GL_ELEMENT_ARRAY_BUFFER.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);

    // Alocamos memória para o buffer.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), NULL, GL_STATIC_DRAW);

    // Copiamos os valores do array indices[] para dentro do buffer.
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

    // NÃO faça a chamada abaixo! Diferente de um VBO (GL_ARRAY_BUFFER), um
    // array de índices (GL_ELEMENT_ARRAY_BUFFER) não pode ser "desligado",
    // caso contrário o VAO irá perder a informação sobre os índices.
    //
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // XXX Errado!
    //

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);

    // Retornamos o ID do VAO. Isso é tudo que será necessário para renderizar
    // os triângulos definidos acima. Veja a chamada glDrawElements() em main().
    return vertex_array_object_id;
}

void AddPlayer(glm::mat4 model){

    // Desenhamos o jogador
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, GREEK_BODY);
    DrawVirtualObject("greek_0");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, GREEK_HEAD);
    DrawVirtualObject("greek_1");
}

void MovePlayer(){
    float angle_45 = M_PI / 4;
    float repele_player = 0.05;

    if(free_camera == false){
        if(g_AngleX_leg > angle_45){
            movement_AngleX_leg = true;
        }
        if(g_AngleX_leg < -angle_45){
            movement_AngleX_leg = false;
        }
        if(g_WKeyPressed == true || g_SKeyPressed == true){
            g_AngleX_leg = g_AngleX_leg + (movement_AngleX_leg ? -speed * delta_t : speed * delta_t);
        }

        // Calculo o angulo entre um vetor saindo do personagem e apontando para X e o vetor da câmera
        glm::vec4 plano_X   = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec4 plano_Z   = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
        glm::vec4 view_aux  = glm::vec4(camera_view_vector.x, 0.0f, camera_view_vector.z, 0.0f);

        float angulo_X;
        float angulo_Z;

        if(movement_restricted == false){
            angulo_X = dotproduct(plano_X, view_aux) / (norm(plano_X) * norm(view_aux));
            angulo_Z = dotproduct(plano_Z, view_aux) / (norm(plano_Z) * norm(view_aux));
        }else{
            angulo_X = dotproduct(plano_X, movement_normal) / (norm(plano_X) * norm(movement_normal));
            angulo_Z = dotproduct(plano_Z, movement_normal) / (norm(plano_Z) * norm(movement_normal));
        }

        // Erro: função acos(x) retorna NaN (Not a Number) para -1 > x or x > 1
        if(angulo_X < -1.0f)
        {
            angulo_X = acos(-1.0f);
        }
        else if(angulo_X > 1.0f)
        {
            angulo_X = acos(1.0f);
        }
        else{
            angulo_X = acos(angulo_X);
        }

        if(angulo_Z < -1.0f)
        {
            angulo_Z = acos(-1.0f);
        }
        else if(angulo_Z > 1.0f)
        {
            angulo_Z = acos(1.0f);
        }
        else{
            angulo_Z = acos(angulo_Z);
        }

        if(movement_restricted == false){
            g_AngleY_torso = angulo_X;
        }

        //------------------------------------------------------

        int movement_signal = 1.0;
        if(g_WKeyPressed == true){
            if(movement_restricted == true){
                movement_restricted = false;
                movement_signal = 1.0f;
            }else{
                movement_signal = 1.0f;
            }

            g_TorsoPositionZ = g_TorsoPositionZ + cos(angulo_Z) * movement_signal * speed * delta_t;
            g_TorsoPositionX = g_TorsoPositionX + cos(angulo_X) * movement_signal * speed * delta_t;
        }
        // else if(g_SKeyPressed == true){
        //     if(movement_restricted == true){
        //         movement_restricted = false;

        //             movement_signal = -1.0f;

        //     }else{
        //         movement_signal = -1.0f;
        //     }

        //     g_TorsoPositionZ = g_TorsoPositionZ + cos(angulo_Z) * movement_signal * speed * delta_t;
        //     g_TorsoPositionX = g_TorsoPositionX + cos(angulo_X) * movement_signal * speed * delta_t;
        // }
        if(g_AKeyPressed == true){
            g_CameraTheta = g_CameraTheta + speed * delta_t;
        }
        if(g_DKeyPressed == true){
            g_CameraTheta = g_CameraTheta - speed * delta_t;
        }
        if(g_COMMAKeyPressed == true)
        {
            g_TorsoPositionY -= speed*delta_t;
        }
        if(g_PERIODKeyPressed == true)
        {
            g_TorsoPositionY += speed*delta_t;
        }
    }else{
        if(g_WKeyPressed == true){
            camera_position_c = camera_position_c - w * speed * delta_t;
        }if(g_AKeyPressed == true){
            camera_position_c = camera_position_c - u * speed * delta_t;
        }if(g_SKeyPressed == true){
            camera_position_c = camera_position_c + w * speed * delta_t;
        }if(g_DKeyPressed == true){
            camera_position_c = camera_position_c + u * speed * delta_t;
        }
    }
}

void AddCave(){

    glm::mat4 model = Matrix_Identity();

    // Desenhamos paredes da caverna
    model = Matrix_Rotate_X(-M_PI_2)
            * Matrix_Scale(2.0f,2.0f,2.0f);
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, CAVE_WALLS);
    DrawVirtualObject("cave_wall");

    model = Matrix_Rotate_X(-M_PI_2)
            * Matrix_Rotate_Z(M_PI)
            * Matrix_Translate(0.0f,22.0f,0.0f)
            * Matrix_Scale(2.0f,2.0f,2.0f);
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, CAVE_WALLS);
    DrawVirtualObject("cave_wall");

    // Desenhamos o chão da caverna
    model = Matrix_Rotate_X(-M_PI_2)
        * Matrix_Translate(0.0f,0.0f,-0.4f)
        * Matrix_Scale(2.0f,2.0f,2.0f);
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, CAVE_FLOOR);
    DrawVirtualObject("cave_floor");

    model = Matrix_Rotate_X(-M_PI_2)
            * Matrix_Rotate_Z(M_PI)
            * Matrix_Translate(0.0f,27.999f,-0.4f)
            * Matrix_Scale(2.0f,2.0f,2.0f);
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, CAVE_FLOOR);
    DrawVirtualObject("cave_floor");

    // Desenhamos o teto da caverna
    model = Matrix_Scale(-17.0f,17.0f,17.0f);
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, GRUTA_BASE);
    DrawVirtualObject("GRUTA_BASE");

    /*// Desenhamos o teto da caverna
    model = Matrix_Scale(17.0f,17.0f,17.0f);
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, GRUTA_BASE);
    DrawVirtualObject("GRUTA_BASE");*/

}

void AddCavePhysics(){

    // Permite o desenho de objetos transparentes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 model = Matrix_Identity();

    // Desenhamos o plano da caverna
    model = Matrix_Rotate_X(-M_PI_2)
            * Matrix_Translate(0.0f,-2.75f,0.0f)
            * Matrix_Scale(1.75f,2.0f,2.0f);
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, CAVE);
    DrawVirtualObject("cave_map");
    collision_player_plane_points(model, 0, &cavemodel, g_TorsoPositionX, g_TorsoPositionY, g_TorsoPositionZ, camera_view_vector, &movement_restricted, &movement_normal);

    model = Matrix_Rotate_X(-M_PI_2)
            * Matrix_Rotate_Z(M_PI)
            * Matrix_Translate(0.0f,19.75f,0.0f)
            * Matrix_Scale(1.75f,2.0f,2.0f);
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, CAVE);
    DrawVirtualObject("cave_map");
    collision_player_plane_points(model, 0, &cavemodel, g_TorsoPositionX, g_TorsoPositionY, g_TorsoPositionZ, camera_view_vector, &movement_restricted, &movement_normal);

}

void AddLadder(glm::mat4 model){

    // Desenhamos a escada
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, LADDER);
    DrawVirtualObject("ladder_ladder_material_0");
    collision_player_box_points(model, "ladder_ladder_material_0", g_TorsoPositionX, g_TorsoPositionY, g_TorsoPositionZ, &movement_restricted, &movement_normal, g_VirtualScene);
    if(interact_radius(model, "ladder_ladder_material_0", 2.0f) == true){
        g_LadderCollision = true;
    }
    else{
        g_LadderCollision = false;
    }

}

void AddTitle(glm::mat4 model){

    // Desenhamos o título do jogo
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, TITLE);
    DrawVirtualObject("title");

}

void AddLettering(glm::mat4 model, int ending){

    int endingID;

    switch(ending)
    {
        case 0:
            endingID = LETTERING;
            break;
        case 1:
            endingID = ENDING1;
            break;
        case 2:
            endingID = ENDING2;
            break;
        case 3:
            endingID = ENDING3;
            break;
        case 4:
            endingID = ENDING4;
            break;
        case 5:
            endingID = ENDING5;
            break;
        case 6:
            endingID = ENDING6;
            break;
        case 7:
            endingID = ENDING7;
            break;
        case 8:
            endingID = ENDING8;
            break;
        case 9:
            endingID = ENDING9;
            break;

    }

    // Desenhamos o lettering
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, endingID);
    DrawVirtualObject("lettering");

}

void AddPrisioner(glm::mat4 model, bool showBody, bool showRest){

    if(showBody)
    {
         // Desenhamos o prisioneiro
        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER);
        DrawVirtualObject("Promitheus_Promitheus_0");

        // Desenhamos o olho esquerdo do prisioneiro
        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_EYES);
        DrawVirtualObject("eyeL_eyes_0");

        // Desenhamos o olho direito do prisioneiro
        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_EYES);
        DrawVirtualObject("eyeR_eyes_0");

        // Desenhamos os dentes superiores do prisioneiro
        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_TEETH);
        DrawVirtualObject("Teeth_up_teeth_0");

        // Desenhamos os dentes inferiores do prisioneiro
        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_TEETH);
        DrawVirtualObject("Teeth_down_teeth_0");

        if(interact_radius(model, "Promitheus_Promitheus_0", 2.0f) == true){
        g_PrisionerRockCollision = true;
        }
        else{
            g_PrisionerRockCollision = false;
        }
    }

    if(showRest)
    {
                // Desenhamos a pedra onde o prisioneiro fica preso
        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_ROCK);
        DrawVirtualObject("Rock_Rock_0");
        collision_player_box_points(model, "Rock_Rock_0", g_TorsoPositionX, g_TorsoPositionY, g_TorsoPositionZ, &movement_restricted, &movement_normal, g_VirtualScene);

        // Desenhamos as partes das correntes que prendem o prisioneiro
        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_xeiropeda_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_xeiropeda001_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos001_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos002_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos003_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos004_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos005_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos006_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos007_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos008_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos009_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos010_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos011_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos012_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos013_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos014_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos015_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos016_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos017_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos018_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos019_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos020_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos021_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos022_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos023_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos024_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos025_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos026_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos027_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos028_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos029_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos030_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos031_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos032_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos033_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos034_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos035_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos036_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos037_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos038_Chain_0");

        glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, PRISIONER_CHAIN);
        DrawVirtualObject("Chain_krikos039_Chain_0");

    }

}

void AddCampfire(glm::mat4 model){

    //Desenhamos a fogueira
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, CAMPFIRE);
    DrawVirtualObject("Campfire");
    collision_player_sphere_points(model, "Campfire", 1.3f, g_TorsoPositionX, g_TorsoPositionY, g_TorsoPositionZ, &movement_restricted, &movement_normal, g_VirtualScene);

    // Permite o desenho de objetos transparentes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    model = model * Matrix_Translate(0.0f,-2.0f,0.0f)
        * Matrix_Scale(2.0f,2.0f,2.0f);

    // Desenhamos 28 partes do fogo
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_00");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_01");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_02");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_03");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_04");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_05");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_06");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_07");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_08");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_09");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_10");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_11");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_12");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_13");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_14");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_15");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_16");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_17");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_18");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_19");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_20");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_21");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_22");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_23");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_24");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_25");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_26");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_27");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, FIRE);
    DrawVirtualObject("fire_part_28");

}

void AddGreek2(glm::mat4 model, GLFWwindow* window){

    // Desenhamos o segundo modelo de grego
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, GREEK2);
    DrawVirtualObject("pericles");
    collision_player_box_points(model, "pericles", g_TorsoPositionX, g_TorsoPositionY, g_TorsoPositionZ, &movement_restricted, &movement_normal, g_VirtualScene);
    if(interact_radius(model, "pericles", 2.0f) == true){
        g_Greek2Collision = true;
    }
    else{
        g_Greek2Collision = false;
    }
}

void AddCaveEntrance(glm::mat4 model){

    // Desenhamos a entrada da caverna
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, CAVE_ENTRANCE2);
    DrawVirtualObject("cave_entrance_01");

    model = model * Matrix_Scale(1.0f,1.0f,0.5f);
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, CAVE_ENTRANCE1);
    DrawVirtualObject("cave_entrance_00");

}

void AddMountain(glm::mat4 model){

    // Desenhamos a montanha
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, MOUNTAIN);
    DrawVirtualObject("mountain_0");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, MOUNTAIN);
    DrawVirtualObject("mountain_1");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, MOUNTAIN);
    DrawVirtualObject("mountain_2");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, MOUNTAIN);
    DrawVirtualObject("mountain_3");

    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, MOUNTAIN);
    DrawVirtualObject("mountain_4");

}

void AddRabbit(glm::mat4 model){
    // Desenhamos a grama
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, RABBIT);
    DrawVirtualObject("rabbit");
}

void AddBear(glm::mat4 model){
    // Desenhamos a grama
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, BEAR);
    DrawVirtualObject("bear");
}

bool interact_radius(glm::mat4 model, char* object, float radius_expand){
    bool colision = false;

     // Centro do Jogador
    glm::vec4 player_center_point = glm::vec4(g_TorsoPositionX, g_TorsoPositionY + 1.0f, g_TorsoPositionZ, 1.0f);

    // Procura e calcula coordenadas mínima e máxima do modelo (aplicando matriz de transformação)
    glm::vec3 bbox_min = g_VirtualScene[object].bbox_min;
    glm::vec3 bbox_max = g_VirtualScene[object].bbox_max;

    glm::vec4 coords_min =  glm::vec4(bbox_min.x - 10, bbox_min.y - 10, bbox_min.z - 10, 1.0f);
    glm::vec4 coords_max =  glm::vec4(bbox_max.x + 10, bbox_max.y + 10, bbox_max.z + 10, 1.0f);

    auto coords_min_T = model * (coords_min);
    auto coords_max_T = model * coords_max;

    // Verifica se o player entra nos limites do cubo
    if(
           player_center_point.x >= coords_min_T.x - radius_expand
        && player_center_point.y >= coords_min_T.y - radius_expand
        && player_center_point.z >= coords_min_T.z - radius_expand

        && player_center_point.x <= coords_max_T.x + radius_expand
        && player_center_point.y <= coords_max_T.y + radius_expand
        && player_center_point.z <= coords_max_T.z + radius_expand
    ){
        colision = true;
    }
    return colision;
}

// Escrevemos na tela o texto da tela inicial.
void TextRendering_ShowInitialScreenText(GLFWwindow* window, char* mensagem, float scale)
{

    float lineheight = TextRendering_LineHeight(window);
    float charwidth = TextRendering_CharWidth(window);

    TextRendering_PrintString(window, mensagem,  - (strlen(mensagem)*charwidth*scale) / 2 , -0.025f, scale);
    //TextRendering_PrintString(window, "Perspective", 1.0f-13*charwidth, -1.0f+2*lineheight/10, 1.0f);
}

// Escrevemos na tela as conversas e opções de interação.
void TextRendering_ShowChatCharacters(GLFWwindow* window, char* mensagem, float scale, float position)
{

    float lineheight = TextRendering_LineHeight(window);
    float charwidth = TextRendering_CharWidth(window);

    TextRendering_PrintString(window, mensagem,  - (strlen(mensagem)*charwidth*scale) / 2 , position, scale);
    //TextRendering_PrintString(window, "Perspective", 1.0f-13*charwidth, -1.0f+2*lineheight/10, 1.0f);
}

// Escrevemos comandos inferiores
void TextRendering_ShowInferiorCommand(GLFWwindow* window, char* mensagem, float scale)
{

    float lineheight = TextRendering_LineHeight(window);
    float charwidth = TextRendering_CharWidth(window);

    TextRendering_PrintString(window, mensagem, 1.0f - ((strlen(mensagem)+1)*charwidth*scale) , lineheight-1.0f, scale);
}

glm::vec4 bezier_curve_two_degree(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, float t){

    glm::vec4 c12 = glm::vec4(
        p1.x + (t * (p2.x - p1.x)),
        p1.y + (t * (p2.y - p1.y)),
        p1.z + (t * (p2.z - p1.z)),
        1.0f
    );

    glm::vec4 c23 = glm::vec4(
        p2.x + (t * (p3.x - p2.x)),
        p2.y + (t * (p3.y - p2.y)),
        p2.z + (t * (p3.z - p2.z)),
        1.0f
    );

    glm::vec4 c_t = glm::vec4(
        c12.x + (t * (c23.x - c12.x)),
        c12.y + (t * (c23.y - c12.y)),
        c12.z + (t * (c23.z - c12.z)),
        1.0f
    );

    return c_t;
}


// set makeprg=cd\ ..\ &&\ make\ run\ >/dev/null
// vim: set spell spelllang=pt_br :

