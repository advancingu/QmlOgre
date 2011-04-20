#include <RenderSystems/GL/OgreGLTexture.h>
#include <RenderSystems/GL/OgreGLFrameBufferObject.h>
#include <RenderSystems/GL/OgreGLFBORenderTexture.h>

#include "ogrenode.h"
#include "cameranodeobject.h"

#include <Ogre.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtOpenGL/QGLFunctions>

static QString appPath()
{
    QString path = QCoreApplication::applicationDirPath();
    QDir dir(path);
#ifdef Q_WS_MAC
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
#elif defined(Q_WS_WIN)
    dir.cdUp();
#endif
    return dir.absolutePath();
}

OgreNode::OgreNode()
    : QSGGeometryNode()
    , m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4)
    , m_texture(0)
    , m_samples(0)
    , m_AAEnabled(false)
    , m_renderTexture(0)
    , m_ogreFBO(0)
    , m_initialized(false)
    , m_dirtyFBO(false)
{
    setMaterial(&m_materialO);
    setOpaqueMaterial(&m_material);
    setGeometry(&m_geometry);
    setFlag(UsePreprocess);
}

OgreNode::~OgreNode()
{
    if (m_renderTexture) {
        m_renderTexture->removeAllViewports();
    }

    if (m_root) {
        m_root->detachRenderTarget(m_renderTexture);

        if (m_sceneManager) {
            m_root->destroySceneManager(m_sceneManager);
        }
    }

    delete m_root;
}

static QString textureUnitToString(GLint texU)
{
    QString actTexStr;
    switch (texU) {
    case GL_TEXTURE0:
        actTexStr = "GL_TEXTURE0";
        break;
    case GL_TEXTURE1:
        actTexStr = "GL_TEXTURE1";
        break;
    case GL_TEXTURE2:
        actTexStr = "GL_TEXTURE2";
        break;
    case GL_TEXTURE3:
        actTexStr = "GL_TEXTURE3";
        break;
    case GL_TEXTURE4:
        actTexStr = "GL_TEXTURE4";
        break;
    case GL_TEXTURE5:
        actTexStr = "GL_TEXTURE5";
        break;
    case GL_TEXTURE6:
        actTexStr = "GL_TEXTURE6";
        break;
    case GL_TEXTURE7:
        actTexStr = "GL_TEXTURE7";
        break;
    case GL_TEXTURE8:
        actTexStr = "GL_TEXTURE8";
        break;
    case GL_TEXTURE9:
        actTexStr = "GL_TEXTURE9";
        break;
    case GL_TEXTURE10:
        actTexStr = "GL_TEXTURE10";
        break;
    case GL_TEXTURE11:
        actTexStr = "GL_TEXTURE11";
        break;
    case GL_TEXTURE12:
        actTexStr = "GL_TEXTURE12";
        break;
    }
    return actTexStr;
}

static QString blendFuncToString(GLint blenFunc)
{
    QString str;
    switch (blenFunc) {
    case GL_ZERO:
        str = "GL_ZERO";
        break;
    case GL_ONE:
        str = "GL_ONE";
        break;
    case GL_SRC_COLOR:
        str = "GL_SRC_COLOR";
        break;
    case GL_ONE_MINUS_SRC_COLOR:
        str = "GL_ONE_MINUS_SRC_COLOR";
        break;
    case GL_DST_COLOR:
        str = "GL_DST_COLOR";
        break;
    case GL_ONE_MINUS_DST_COLOR:
        str = "GL_ONE_MINUS_DST_COLOR";
        break;
    case GL_SRC_ALPHA:
        str = "GL_SRC_ALPHA";
        break;
    case GL_ONE_MINUS_SRC_ALPHA:
        str = "GL_ONE_MINUS_SRC_ALPHA";
        break;
    case GL_DST_ALPHA:
        str = "GL_DST_ALPHA";
        break;
    case GL_ONE_MINUS_DST_ALPHA:
        str = "GL_ONE_MINUS_DST_ALPHA";
        break;
    case GL_CONSTANT_COLOR:
        str = "GL_CONSTANT_COLOR";
        break;
    case GL_ONE_MINUS_CONSTANT_COLOR:
        str = "GL_ONE_MINUS_CONSTANT_COLOR";
        break;
    case GL_CONSTANT_ALPHA:
        str = "GL_CONSTANT_ALPHA";
        break;
    case GL_ONE_MINUS_CONSTANT_ALPHA:
        str = "GL_ONE_MINUS_CONSTANT_ALPHA";
        break;
    }
    return str;
}

static QString blendEqToString(GLint blendEq)
{
    QString str;
    switch (blendEq) {
    case GL_FUNC_ADD:
        str = "GL_FUNC_ADD";
        break;
    case GL_FUNC_SUBTRACT:
        str = "GL_FUNC_SUBTRACT";
        break;
    case GL_FUNC_REVERSE_SUBTRACT:
        str = "GL_FUNC_REVERSE_SUBTRACT";
        break;
    case GL_MIN:
        str = "GL_MIN";
        break;
    case GL_MAX:
        str = "GL_MAX";
        break;
    }
    return str;
}

static QString funcToString(GLint func)
{
    QString str;
    switch (func) {
    case GL_NEVER:
        str = "GL_NEVER";
        break;
    case GL_LESS:
        str = "GL_LESS";
        break;
    case GL_EQUAL:
        str = "GL_EQUAL";
        break;
    case GL_LEQUAL:
        str = "GL_LEQUAL";
        break;
    case GL_GREATER:
        str = "GL_GREATER";
        break;
    case GL_NOTEQUAL:
        str = "GL_NOTEQUAL";
        break;
    case GL_GEQUAL:
        str = "GL_GEQUAL";
        break;
    case GL_ALWAYS:
        str = "GL_ALWAYS";
        break;
    }
    return str;
}

static QString drawBufferToString(GLint val)
{
    QString str;
    switch (val) {
    case GL_NONE:
        str = "GL_NONE";
        break;
    case GL_FRONT_LEFT:
        str = "GL_FRONT_LEFT";
        break;
    case GL_FRONT_RIGHT:
        str = "GL_FRONT_RIGHT";
        break;
    case GL_BACK_LEFT:
        str = "GL_BACK_LEFT";
        break;
    case GL_BACK_RIGHT:
        str = "GL_BACK_RIGHT";
        break;
    case GL_FRONT:
        str = "GL_FRONT";
        break;
    case GL_BACK:
        str = "GL_BACK";
        break;
    case GL_LEFT:
        str = "GL_LEFT";
        break;
    case GL_RIGHT:
        str = "GL_RIGHT";
        break;
    case GL_FRONT_AND_BACK:
        str = "GL_FRONT_AND_BACK";
        break;
    }
    return str;
}

static QString hintBehavToString(GLint val)
{
    QString str;
    switch (val) {
    case GL_FASTEST:
        str = "GL_FASTEST";
        break;
    case GL_NICEST:
        str = "GL_NICEST";
        break;
    case GL_DONT_CARE:
        str = "GL_DONT_CARE";
        break;
    }
    return str;
}

static QString pixFormatToString(GLint val)
{
    QString str;
    switch (val) {
    case GL_STENCIL_INDEX:
        str = "GL_STENCIL_INDEX";
        break;
    case GL_DEPTH_COMPONENT:
        str = "GL_DEPTH_COMPONENT";
        break;
    case GL_DEPTH_STENCIL:
        str = "GL_DEPTH_STENCIL";
        break;
    case GL_RED:
        str = "GL_RED";
        break;
    case GL_GREEN:
        str = "GL_GREEN";
        break;
    case GL_BLUE:
        str = "GL_BLUE";
        break;
    case GL_RGB:
        str = "GL_RGB";
        break;
    case GL_BGR:
        str = "GL_BGR";
        break;
    case GL_RGBA:
        str = "GL_RGBA";
        break;
    case GL_BGRA:
        str = "GL_BGRA";
        break;
    }
    return str;
}

static QString pixTypeToString(GLint val)
{
    QString str;
    switch (val) {
    case GL_UNSIGNED_BYTE:
        str = "GL_UNSIGNED_BYTE";
        break;
    case GL_BYTE:
        str = "GL_BYTE";
        break;
    case GL_UNSIGNED_SHORT:
        str = "GL_UNSIGNED_SHORT";
        break;
    case GL_SHORT:
        str = "GL_SHORT";
        break;
    case GL_UNSIGNED_INT:
        str = "GL_UNSIGNED_INT";
        break;
    case GL_INT:
        str = "GL_INT";
        break;
    case GL_HALF_FLOAT:
        str = "GL_HALF_FLOAT";
        break;
    case GL_FLOAT:
        str = "GL_FLOAT";
        break;
    case GL_UNSIGNED_BYTE_3_3_2:
        str = "GL_UNSIGNED_BYTE_3_3_2";
        break;
    case GL_UNSIGNED_BYTE_2_3_3_REV:
        str = "GL_UNSIGNED_BYTE_2_3_3_REV";
        break;
    case GL_UNSIGNED_SHORT_5_6_5:
        str = "GL_UNSIGNED_SHORT_5_6_5";
        break;
    case GL_UNSIGNED_SHORT_5_6_5_REV:
        str = "GL_UNSIGNED_SHORT_5_6_5_REV";
        break;
    case GL_UNSIGNED_SHORT_4_4_4_4:
        str = "GL_UNSIGNED_SHORT_4_4_4_4";
        break;
    case GL_UNSIGNED_SHORT_4_4_4_4_REV:
        str = "GL_UNSIGNED_SHORT_4_4_4_4_REV";
        break;
    case GL_UNSIGNED_SHORT_5_5_5_1:
        str = "GL_UNSIGNED_SHORT_5_5_5_1";
        break;
    case GL_UNSIGNED_SHORT_1_5_5_5_REV:
        str = "GL_UNSIGNED_SHORT_1_5_5_5_REV";
        break;
    case GL_UNSIGNED_INT_8_8_8_8:
        str = "GL_UNSIGNED_INT_8_8_8_8";
        break;
    case GL_UNSIGNED_INT_8_8_8_8_REV:
        str = "GL_UNSIGNED_INT_8_8_8_8_REV";
        break;
    case GL_UNSIGNED_INT_10_10_10_2:
        str = "GL_UNSIGNED_INT_10_10_10_2";
        break;
    case GL_UNSIGNED_INT_2_10_10_10_REV:
        str = "GL_UNSIGNED_INT_2_10_10_10_REV";
        break;
    case GL_UNSIGNED_INT_24_8:
        str = "GL_UNSIGNED_INT_24_8";
        break;
    case GL_UNSIGNED_INT_10F_11F_11F_REV:
        str = "GL_UNSIGNED_INT_10F_11F_11F_REV";
        break;
    case GL_UNSIGNED_INT_5_9_9_9_REV:
        str = "GL_UNSIGNED_INT_5_9_9_9_REV";
        break;
    case GL_FLOAT_32_UNSIGNED_INT_24_8_REV:
        str = "GL_FLOAT_32_UNSIGNED_INT_24_8_REV";
        break;
    }
    return str;
}

static QString pointParameterToString(GLint val)
{
    QString str;
    switch (val) {
    case GL_POINT_FADE_THRESHOLD_SIZE:
        str = "GL_POINT_FADE_THRESHOLD_SIZE";
        break;
    case GL_POINT_SPRITE_COORD_ORIGIN:
        str = "GL_POINT_SPRITE_COORD_ORIGIN";
        break;
    }
    return str;
}

static QString logicOpToString(GLint val)
{
    QString str;
    switch (val) {
    case GL_CLEAR:
        str = "GL_CLEAR";
        break;
    case GL_SET:
        str = "GL_SET";
        break;
    case GL_COPY:
        str = "GL_COPY";
        break;
    case GL_COPY_INVERTED:
        str = "GL_COPY_INVERTED";
        break;
    case GL_NOOP:
        str = "GL_NOOP";
        break;
    case GL_INVERT:
        str = "GL_INVERT";
        break;
    case GL_AND:
        str = "GL_AND";
        break;
    case GL_NAND:
        str = "GL_NAND";
        break;
    case GL_OR:
        str = "GL_OR";
        break;
    case GL_NOR:
        str = "GL_NOR";
        break;
    case GL_XOR:
        str = "GL_XOR";
        break;
    case GL_EQUIV:
        str = "GL_EQUIV";
        break;
    case GL_AND_REVERSE:
        str = "GL_AND_REVERSE";
        break;
    case GL_AND_INVERTED:
        str = "GL_AND_INVERTED";
        break;
    case GL_OR_REVERSE:
        str = "GL_OR_REVERSE";
        break;
    case GL_OR_INVERTED:
        str = "GL_OR_INVERTED";
        break;
    }
    return str;
}

static QString provokVertexToString(GLint val)
{
    QString str;
    switch (val) {
    case GL_FIRST_VERTEX_CONVENTION:
        str = "GL_FIRST_VERTEX_CONVENTION";
        break;
    case GL_LAST_VERTEX_CONVENTION:
        str = "GL_LAST_VERTEX_CONVENTION";
        break;
    }
    return str;
}

static QString sfailToString(GLint val)
{
    QString str;
    switch (val) {
    case GL_KEEP:
        str = "GL_KEEP";
        break;
    case GL_ZERO:
        str = "GL_ZERO";
        break;
    case GL_REPLACE:
        str = "GL_REPLACE";
        break;
    case GL_INCR:
        str = "GL_INCR";
        break;
    case GL_INCR_WRAP:
        str = "GL_INCR_WRAP";
        break;
    case GL_DECR:
        str = "GL_DECR";
        break;
    case GL_DECR_WRAP:
        str = "GL_DECR_WRAP";
        break;
    case GL_INVERT:
        str = "GL_INVERT";
        break;
    }
    return str;
}


static void printGLState()
{
    qDebug() << "====== current GL state ======";

    GLint activeTexture;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTexture);
    qDebug() << "GL_ACTIVE_TEXTURE =" << textureUnitToString(activeTexture);

    GLfloat ALIASED_LINE_WIDTH_RANGE[2];
    glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, &ALIASED_LINE_WIDTH_RANGE[0]);
    qDebug() << "GL_ALIASED_LINE_WIDTH_RANGE =" << ALIASED_LINE_WIDTH_RANGE[0] << "-->" << ALIASED_LINE_WIDTH_RANGE[1];

    GLint ARRAY_BUFFER_BINDING;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &ARRAY_BUFFER_BINDING);
    qDebug() << "GL_ARRAY_BUFFER_BINDING =" << ARRAY_BUFFER_BINDING;

    GLboolean BLEND;
    glGetBooleanv(GL_BLEND, &BLEND);
    qDebug() << "GL_BLEND =" << (BLEND ? "true" : "false");

    GLfloat BLEND_COLOR[4];
    glGetFloatv(GL_BLEND_COLOR, &BLEND_COLOR[0]);
    qDebug() << "GL_BLEND_COLOR = Color(" << BLEND_COLOR[0] << BLEND_COLOR[1] << BLEND_COLOR[2] << BLEND_COLOR[3] << ")";

    GLint BLEND_DST_ALPHA;
    glGetIntegerv(GL_BLEND_DST_ALPHA, &BLEND_DST_ALPHA);
    qDebug() << "GL_BLEND_DST_ALPHA =" << blendFuncToString(BLEND_DST_ALPHA);

    GLint BLEND_DST_RGB;
    glGetIntegerv(GL_BLEND_DST_RGB, &BLEND_DST_RGB);
    qDebug() << "GL_BLEND_DST_RGB =" << blendFuncToString(BLEND_DST_RGB);

    GLint BLEND_EQUATION_RGB;
    glGetIntegerv(GL_BLEND_EQUATION_RGB, &BLEND_EQUATION_RGB);
    qDebug() << "GL_BLEND_EQUATION_RGB =" << blendEqToString(BLEND_EQUATION_RGB);

    GLint BLEND_EQUATION_ALPHA;
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &BLEND_EQUATION_ALPHA);
    qDebug() << "GL_BLEND_EQUATION_ALPHA =" << blendEqToString(BLEND_EQUATION_ALPHA);

    GLint BLEND_SRC_ALPHA;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &BLEND_SRC_ALPHA);
    qDebug() << "GL_BLEND_SRC_ALPHA =" << blendFuncToString(BLEND_SRC_ALPHA);

    GLint BLEND_SRC_RGB;
    glGetIntegerv(GL_BLEND_SRC_RGB, &BLEND_SRC_RGB);
    qDebug() << "GL_BLEND_SRC_RGB =" << blendFuncToString(BLEND_SRC_RGB);

    GLfloat COLOR_CLEAR_VALUE[4];
    glGetFloatv(GL_COLOR_CLEAR_VALUE, &COLOR_CLEAR_VALUE[0]);
    qDebug() << "GL_COLOR_CLEAR_VALUE = Color(" << COLOR_CLEAR_VALUE[0] << COLOR_CLEAR_VALUE[1] << COLOR_CLEAR_VALUE[2] << COLOR_CLEAR_VALUE[3] << ")";

    GLboolean COLOR_LOGIC_OP;
    glGetBooleanv(GL_COLOR_LOGIC_OP, &COLOR_LOGIC_OP);
    qDebug() << "GL_COLOR_LOGIC_OP =" << (COLOR_LOGIC_OP ? "true" : "false");

    GLboolean COLOR_WRITEMASK[4];
    glGetBooleanv(GL_COLOR_WRITEMASK, &COLOR_WRITEMASK[0]);
    qDebug() << "GL_COLOR_WRITEMASK =" << COLOR_WRITEMASK[0] << COLOR_WRITEMASK[1] << COLOR_WRITEMASK[2] << COLOR_WRITEMASK[3];

    GLboolean CULL_FACE;
    glGetBooleanv(GL_CULL_FACE, &CULL_FACE);
    qDebug() << "GL_CULL_FACE =" << (CULL_FACE ? "true" : "false");

    GLint CURRENT_PROGRAM;
    glGetIntegerv(GL_CURRENT_PROGRAM, &CURRENT_PROGRAM);
    qDebug() << "GL_CURRENT_PROGRAM =" << CURRENT_PROGRAM;

    GLfloat DEPTH_CLEAR_VALUE;
    glGetFloatv(GL_DEPTH_CLEAR_VALUE, &DEPTH_CLEAR_VALUE);
    qDebug() << "GL_DEPTH_CLEAR_VALUE =" << DEPTH_CLEAR_VALUE;

    GLint DEPTH_FUNC;
    glGetIntegerv(GL_DEPTH_FUNC, &DEPTH_FUNC);
    qDebug() << "GL_DEPTH_FUNC =" << funcToString(DEPTH_FUNC);

    GLfloat DEPTH_RANGE[2];
    glGetFloatv(GL_DEPTH_RANGE, &DEPTH_RANGE[0]);
    qDebug() << "GL_DEPTH_RANGE =" << DEPTH_RANGE[0] << "-->" << DEPTH_RANGE[1];

    GLboolean DEPTH_TEST;
    glGetBooleanv(GL_DEPTH_TEST, &DEPTH_TEST);
    qDebug() << "GL_DEPTH_TEST =" << (DEPTH_TEST ? "true" : "false");

    GLboolean DEPTH_WRITEMASK;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &DEPTH_WRITEMASK);
    qDebug() << "GL_DEPTH_WRITEMASK =" << (DEPTH_WRITEMASK ? "true" : "false");

    GLboolean DITHER;
    glGetBooleanv(GL_DITHER, &DITHER);
    qDebug() << "GL_DITHER =" << (DITHER ? "true" : "false");

    GLint DRAW_BUFFER;
    glGetIntegerv(GL_DRAW_BUFFER, &DRAW_BUFFER);
    qDebug() << "GL_DRAW_BUFFER =" << drawBufferToString(DRAW_BUFFER);

    GLint DRAW_FRAMEBUFFER_BINDING;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &DRAW_FRAMEBUFFER_BINDING);
    qDebug() << "GL_DRAW_FRAMEBUFFER_BINDING =" << DRAW_FRAMEBUFFER_BINDING;

    GLint READ_FRAMEBUFFER_BINDING;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &READ_FRAMEBUFFER_BINDING);
    qDebug() << "GL_READ_FRAMEBUFFER_BINDING =" << READ_FRAMEBUFFER_BINDING;

    GLint ELEMENT_ARRAY_BUFFER_BINDING;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ELEMENT_ARRAY_BUFFER_BINDING);
    qDebug() << "GL_ELEMENT_ARRAY_BUFFER_BINDING =" << ELEMENT_ARRAY_BUFFER_BINDING;

    GLint FRAGMENT_SHADER_DERIVATIVE_HINT;
    glGetIntegerv(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, &FRAGMENT_SHADER_DERIVATIVE_HINT);
    qDebug() << "GL_FRAGMENT_SHADER_DERIVATIVE_HINT =" << hintBehavToString(FRAGMENT_SHADER_DERIVATIVE_HINT);

    GLint IMPLEMENTATION_COLOR_READ_FORMAT;
    glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES, &IMPLEMENTATION_COLOR_READ_FORMAT);
    qDebug() << "GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES =" << pixFormatToString(IMPLEMENTATION_COLOR_READ_FORMAT);

    GLint IMPLEMENTATION_COLOR_READ_TYPE;
    glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_TYPE_OES, &IMPLEMENTATION_COLOR_READ_TYPE);
    qDebug() << "GL_IMPLEMENTATION_COLOR_READ_TYPE_OES =" << pixTypeToString(IMPLEMENTATION_COLOR_READ_TYPE);

    GLboolean LINE_SMOOTH;
    glGetBooleanv(GL_LINE_SMOOTH, &LINE_SMOOTH);
    qDebug() << "GL_LINE_SMOOTH =" << (LINE_SMOOTH ? "true" : "false");

    GLint LINE_SMOOTH_HINT;
    glGetIntegerv(GL_LINE_SMOOTH_HINT, &LINE_SMOOTH_HINT);
    qDebug() << "GL_LINE_SMOOTH_HINT =" << hintBehavToString(LINE_SMOOTH_HINT);

    GLfloat LINE_WIDTH;
    glGetFloatv(GL_LINE_WIDTH, &LINE_WIDTH);
    qDebug() << "GL_LINE_WIDTH =" << LINE_WIDTH;

    GLfloat LINE_WIDTH_GRANULARITY;
    glGetFloatv(GL_LINE_WIDTH_GRANULARITY, &LINE_WIDTH_GRANULARITY);
    qDebug() << "GL_LINE_WIDTH_GRANULARITY =" << LINE_WIDTH_GRANULARITY;

    GLfloat LINE_WIDTH_RANGE[2];
    glGetFloatv(GL_LINE_WIDTH_RANGE, &LINE_WIDTH_RANGE[0]);
    qDebug() << "GL_LINE_WIDTH_RANGE =" << LINE_WIDTH_RANGE[0] << "-->" << LINE_WIDTH_RANGE[1];

    GLint LOGIC_OP_MODE;
    glGetIntegerv(GL_LOGIC_OP_MODE, &LOGIC_OP_MODE);
    qDebug() << "GL_LOGIC_OP_MODE =" << logicOpToString(LOGIC_OP_MODE);

    GLfloat PACK_ALIGNMENT;
    glGetFloatv(GL_PACK_ALIGNMENT, &PACK_ALIGNMENT);
    qDebug() << "GL_PACK_ALIGNMENT =" << PACK_ALIGNMENT;

    GLfloat PACK_IMAGE_HEIGHT;
    glGetFloatv(GL_PACK_IMAGE_HEIGHT, &PACK_IMAGE_HEIGHT);
    qDebug() << "GL_PACK_IMAGE_HEIGHT =" << PACK_IMAGE_HEIGHT;

    GLboolean PACK_LSB_FIRST;
    glGetBooleanv(GL_PACK_LSB_FIRST, &PACK_LSB_FIRST);
    qDebug() << "GL_PACK_LSB_FIRST =" << (PACK_LSB_FIRST ? "true" : "false");

    GLfloat PACK_ROW_LENGTH;
    glGetFloatv(GL_PACK_ROW_LENGTH, &PACK_ROW_LENGTH);
    qDebug() << "GL_PACK_ROW_LENGTH =" << PACK_ROW_LENGTH;

    GLfloat PACK_SKIP_IMAGES;
    glGetFloatv(GL_PACK_SKIP_IMAGES, &PACK_SKIP_IMAGES);
    qDebug() << "GL_PACK_SKIP_IMAGES =" << PACK_SKIP_IMAGES;

    GLfloat PACK_SKIP_PIXELS;
    glGetFloatv(GL_PACK_SKIP_PIXELS, &PACK_SKIP_PIXELS);
    qDebug() << "GL_PACK_SKIP_PIXELS =" << PACK_SKIP_PIXELS;

    GLfloat PACK_SKIP_ROWS;
    glGetFloatv(GL_PACK_SKIP_ROWS, &PACK_SKIP_ROWS);
    qDebug() << "GL_PACK_SKIP_ROWS =" << PACK_SKIP_ROWS;

    GLboolean PACK_SWAP_BYTES;
    glGetBooleanv(GL_PACK_SWAP_BYTES, &PACK_SWAP_BYTES);
    qDebug() << "GL_PACK_SWAP_BYTES =" << (PACK_SWAP_BYTES ? "true" : "false");

    GLint PIXEL_PACK_BUFFER_BINDING;
    glGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING, &PIXEL_PACK_BUFFER_BINDING);
    qDebug() << "GL_PIXEL_PACK_BUFFER_BINDING =" << PIXEL_PACK_BUFFER_BINDING;

    GLint PIXEL_UNPACK_BUFFER_BINDING;
    glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &PIXEL_UNPACK_BUFFER_BINDING);
    qDebug() << "GL_PIXEL_UNPACK_BUFFER_BINDING =" << PIXEL_UNPACK_BUFFER_BINDING;

    GLfloat POINT_FADE_THRESHOLD_SIZE;
    glGetFloatv(GL_POINT_FADE_THRESHOLD_SIZE, &POINT_FADE_THRESHOLD_SIZE);
    qDebug() << "GL_POINT_FADE_THRESHOLD_SIZE =" << POINT_FADE_THRESHOLD_SIZE;

    GLint PRIMITIVE_RESTART_INDEX;
    glGetIntegerv(GL_PRIMITIVE_RESTART_INDEX, &PRIMITIVE_RESTART_INDEX);
    qDebug() << "GL_PRIMITIVE_RESTART_INDEX =" << PRIMITIVE_RESTART_INDEX;

    GLint PROVOKING_VERTEX;
    glGetIntegerv(GL_PROVOKING_VERTEX, &PROVOKING_VERTEX);
    qDebug() << "GL_PROVOKING_VERTEX =" << provokVertexToString(PROVOKING_VERTEX);

    GLfloat POINT_SIZE;
    glGetFloatv(GL_POINT_SIZE, &POINT_SIZE);
    qDebug() << "GL_POINT_SIZE =" << POINT_SIZE;

    GLfloat POINT_SIZE_GRANULARITY;
    glGetFloatv(GL_POINT_SIZE_GRANULARITY, &POINT_SIZE_GRANULARITY);
    qDebug() << "GL_POINT_SIZE_GRANULARITY =" << POINT_SIZE_GRANULARITY;

    GLfloat POINT_SIZE_RANGE[2];
    glGetFloatv(GL_POINT_SIZE_RANGE, &POINT_SIZE_RANGE[0]);
    qDebug() << "GL_POINT_SIZE_RANGE =" << POINT_SIZE_RANGE[0] << "-->" << POINT_SIZE_RANGE[1];

    GLfloat POLYGON_OFFSET_FACTOR;
    glGetFloatv(GL_POLYGON_OFFSET_FACTOR, &POLYGON_OFFSET_FACTOR);
    qDebug() << "GL_POLYGON_OFFSET_FACTOR =" << POLYGON_OFFSET_FACTOR;

    GLfloat POLYGON_OFFSET_UNITS;
    glGetFloatv(GL_POLYGON_OFFSET_UNITS, &POLYGON_OFFSET_UNITS);
    qDebug() << "GL_POLYGON_OFFSET_UNITS =" << POLYGON_OFFSET_UNITS;

    GLboolean POLYGON_OFFSET_FILL;
    glGetBooleanv(GL_POLYGON_OFFSET_FILL, &POLYGON_OFFSET_FILL);
    qDebug() << "GL_POLYGON_OFFSET_FILL =" << (POLYGON_OFFSET_FILL ? "true" : "false");

    GLboolean POLYGON_OFFSET_LINE;
    glGetBooleanv(GL_POLYGON_OFFSET_LINE, &POLYGON_OFFSET_LINE);
    qDebug() << "GL_POLYGON_OFFSET_LINE =" << (POLYGON_OFFSET_LINE ? "true" : "false");

    GLboolean POLYGON_OFFSET_POINT;
    glGetBooleanv(GL_POLYGON_OFFSET_POINT, &POLYGON_OFFSET_POINT);
    qDebug() << "GL_POLYGON_OFFSET_POINT =" << (POLYGON_OFFSET_POINT ? "true" : "false");

    GLboolean POLYGON_SMOOTH;
    glGetBooleanv(GL_POLYGON_SMOOTH, &POLYGON_SMOOTH);
    qDebug() << "GL_POLYGON_SMOOTH =" << (POLYGON_SMOOTH ? "true" : "false");

    GLint POLYGON_SMOOTH_HINT;
    glGetIntegerv(GL_POLYGON_SMOOTH_HINT, &POLYGON_SMOOTH_HINT);
    qDebug() << "GL_POLYGON_SMOOTH_HINT =" << hintBehavToString(POLYGON_SMOOTH_HINT);

    GLint READ_BUFFER;
    glGetIntegerv(GL_READ_BUFFER, &READ_BUFFER);
    qDebug() << "GL_READ_BUFFER =" << drawBufferToString(READ_BUFFER);

    GLint RENDERBUFFER_BINDING;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &RENDERBUFFER_BINDING);
    qDebug() << "GL_RENDERBUFFER_BINDING =" << RENDERBUFFER_BINDING;

    GLint SAMPLE_BUFFERS;
    glGetIntegerv(GL_SAMPLE_BUFFERS, &SAMPLE_BUFFERS);
    qDebug() << "GL_SAMPLE_BUFFERS =" << SAMPLE_BUFFERS;

    GLfloat SAMPLE_COVERAGE_VALUE;
    glGetFloatv(GL_SAMPLE_COVERAGE_VALUE, &SAMPLE_COVERAGE_VALUE);
    qDebug() << "GL_SAMPLE_COVERAGE_VALUE =" << SAMPLE_COVERAGE_VALUE;

    GLboolean SAMPLE_COVERAGE_INVERT;
    glGetBooleanv(GL_SAMPLE_COVERAGE_INVERT, &SAMPLE_COVERAGE_INVERT);
    qDebug() << "GL_SAMPLE_COVERAGE_INVERT =" << (SAMPLE_COVERAGE_INVERT ? "true" : "false");

    GLint SAMPLER_BINDING;
    glGetIntegerv(GL_SAMPLER_BINDING, &SAMPLER_BINDING);
    qDebug() << "GL_SAMPLER_BINDING =" << SAMPLER_BINDING;

    GLint SAMPLES;
    glGetIntegerv(GL_SAMPLES, &SAMPLES);
    qDebug() << "GL_SAMPLES =" << SAMPLES;

    GLint SCISSOR_BOX[4];
    glGetIntegerv(GL_SCISSOR_BOX, &SCISSOR_BOX[0]);
    qDebug() << "GL_SCISSOR_BOX = Rect(" << SCISSOR_BOX[0] << SCISSOR_BOX[1] << SCISSOR_BOX[2] << SCISSOR_BOX[3] << ")";

    GLboolean SCISSOR_TEST;
    glGetBooleanv(GL_SCISSOR_TEST, &SCISSOR_TEST);
    qDebug() << "GL_SCISSOR_TEST =" << (SCISSOR_TEST ? "true" : "false");

    GLint STENCIL_BACK_FAIL;
    glGetIntegerv(GL_STENCIL_BACK_FAIL, &STENCIL_BACK_FAIL);
    qDebug() << "GL_STENCIL_BACK_FAIL =" << sfailToString(STENCIL_BACK_FAIL);

    GLint STENCIL_BACK_FUNC;
    glGetIntegerv(GL_STENCIL_BACK_FUNC, &STENCIL_BACK_FUNC);
    qDebug() << "GL_STENCIL_BACK_FUNC =" << funcToString(STENCIL_BACK_FUNC);

    GLint STENCIL_BACK_PASS_DEPTH_FAIL;
    glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_FAIL, &STENCIL_BACK_PASS_DEPTH_FAIL);
    qDebug() << "GL_STENCIL_BACK_PASS_DEPTH_FAIL =" << sfailToString(STENCIL_BACK_PASS_DEPTH_FAIL);

    GLint STENCIL_BACK_PASS_DEPTH_PASS;
    glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_PASS, &STENCIL_BACK_PASS_DEPTH_PASS);
    qDebug() << "GL_STENCIL_BACK_PASS_DEPTH_PASS =" << sfailToString(STENCIL_BACK_PASS_DEPTH_PASS);

    GLint STENCIL_BACK_REF;
    glGetIntegerv(GL_STENCIL_BACK_REF, &STENCIL_BACK_REF);
    qDebug() << "GL_STENCIL_BACK_REF =" << STENCIL_BACK_REF;

    GLint STENCIL_BACK_VALUE_MASK;
    glGetIntegerv(GL_STENCIL_BACK_VALUE_MASK, &STENCIL_BACK_VALUE_MASK);
    qDebug() << "GL_STENCIL_BACK_VALUE_MASK =" << STENCIL_BACK_VALUE_MASK;

    GLint STENCIL_BACK_WRITEMASK;
    glGetIntegerv(GL_STENCIL_BACK_WRITEMASK, &STENCIL_BACK_WRITEMASK);
    qDebug() << "GL_STENCIL_BACK_WRITEMASK =" << STENCIL_BACK_WRITEMASK;

    GLint STENCIL_CLEAR_VALUE;
    glGetIntegerv(GL_STENCIL_CLEAR_VALUE, &STENCIL_CLEAR_VALUE);
    qDebug() << "GL_STENCIL_CLEAR_VALUE =" << STENCIL_CLEAR_VALUE;

    GLint STENCIL_FAIL;
    glGetIntegerv(GL_STENCIL_FAIL, &STENCIL_FAIL);
    qDebug() << "GL_STENCIL_FAIL =" << sfailToString(STENCIL_FAIL);

    GLint STENCIL_FUNC;
    glGetIntegerv(GL_STENCIL_FUNC, &STENCIL_FUNC);
    qDebug() << "GL_STENCIL_FUNC =" << funcToString(STENCIL_FUNC);

    GLint STENCIL_PASS_DEPTH_FAIL;
    glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, &STENCIL_PASS_DEPTH_FAIL);
    qDebug() << "GL_STENCIL_PASS_DEPTH_FAIL =" << sfailToString(STENCIL_PASS_DEPTH_FAIL);

    GLint STENCIL_PASS_DEPTH_PASS;
    glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, &STENCIL_PASS_DEPTH_PASS);
    qDebug() << "GL_STENCIL_PASS_DEPTH_PASS =" << sfailToString(STENCIL_PASS_DEPTH_PASS);

    GLint STENCIL_REF;
    glGetIntegerv(GL_STENCIL_REF, &STENCIL_REF);
    qDebug() << "GL_STENCIL_REF =" << STENCIL_REF;

    GLboolean STENCIL_TEST;
    glGetBooleanv(GL_STENCIL_TEST, &STENCIL_TEST);
    qDebug() << "GL_STENCIL_TEST =" << (STENCIL_TEST ? "true" : "false");

    GLint STENCIL_VALUE_MASK;
    glGetIntegerv(GL_STENCIL_VALUE_MASK, &STENCIL_VALUE_MASK);
    qDebug() << "GL_STENCIL_VALUE_MASK =" << STENCIL_VALUE_MASK;

    GLint STENCIL_WRITEMASK;
    glGetIntegerv(GL_STENCIL_WRITEMASK, &STENCIL_WRITEMASK);
    qDebug() << "GL_STENCIL_WRITEMASK =" << STENCIL_WRITEMASK;

    GLint SUBPIXEL_BITS;
    glGetIntegerv(GL_SUBPIXEL_BITS, &SUBPIXEL_BITS);
    qDebug() << "GL_SUBPIXEL_BITS =" << SUBPIXEL_BITS;

    GLint TEXTURE_BINDING_1D;
    glGetIntegerv(GL_TEXTURE_BINDING_1D, &TEXTURE_BINDING_1D);
    qDebug() << "GL_TEXTURE_BINDING_1D =" << TEXTURE_BINDING_1D;

    GLint TEXTURE_BINDING_1D_ARRAY;
    glGetIntegerv(GL_TEXTURE_BINDING_1D_ARRAY, &TEXTURE_BINDING_1D_ARRAY);
    qDebug() << "GL_TEXTURE_BINDING_1D_ARRAY =" << TEXTURE_BINDING_1D_ARRAY;

    GLint TEXTURE_BINDING_2D;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &TEXTURE_BINDING_2D);
    qDebug() << "GL_TEXTURE_BINDING_2D =" << TEXTURE_BINDING_2D;

    GLint TEXTURE_BINDING_2D_ARRAY;
    glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &TEXTURE_BINDING_2D_ARRAY);
    qDebug() << "GL_TEXTURE_BINDING_2D_ARRAY =" << TEXTURE_BINDING_2D_ARRAY;

    GLint TEXTURE_BINDING_2D_MULTISAMPLE;
    glGetIntegerv(GL_TEXTURE_BINDING_2D_MULTISAMPLE, &TEXTURE_BINDING_2D_MULTISAMPLE);
    qDebug() << "GL_TEXTURE_BINDING_2D_MULTISAMPLE =" << TEXTURE_BINDING_2D_MULTISAMPLE;

    GLint GTEXTURE_BINDING_2D_MULTISAMPLE_ARRAY;
    glGetIntegerv(GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY, &GTEXTURE_BINDING_2D_MULTISAMPLE_ARRAY);
    qDebug() << "GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY =" << GTEXTURE_BINDING_2D_MULTISAMPLE_ARRAY;

    GLint TEXTURE_BINDING_3D;
    glGetIntegerv(GL_TEXTURE_BINDING_3D, &TEXTURE_BINDING_3D);
    qDebug() << "GL_TEXTURE_BINDING_3D =" << TEXTURE_BINDING_3D;

    GLint TEXTURE_BINDING_BUFFER;
    glGetIntegerv(GL_TEXTURE_BINDING_BUFFER, &TEXTURE_BINDING_BUFFER);
    qDebug() << "GL_TEXTURE_BINDING_BUFFER =" << TEXTURE_BINDING_BUFFER;

    GLint TEXTURE_BINDING_CUBE_MAP;
    glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &TEXTURE_BINDING_CUBE_MAP);
    qDebug() << "GL_TEXTURE_BINDING_CUBE_MAP =" << TEXTURE_BINDING_CUBE_MAP;

    GLint TEXTURE_BINDING_RECTANGLE;
    glGetIntegerv(GL_TEXTURE_BINDING_RECTANGLE, &TEXTURE_BINDING_RECTANGLE);
    qDebug() << "GL_TEXTURE_BINDING_RECTANGLE =" << TEXTURE_BINDING_RECTANGLE;

    GLint TEXTURE_COMPRESSION_HINT;
    glGetIntegerv(GL_TEXTURE_COMPRESSION_HINT, &TEXTURE_COMPRESSION_HINT);
    qDebug() << "GL_TEXTURE_COMPRESSION_HINT =" << hintBehavToString(TEXTURE_COMPRESSION_HINT);

    GLint TRANSFORM_FEEDBACK_BUFFER_BINDING;
    glGetIntegerv(GL_TRANSFORM_FEEDBACK_BUFFER_BINDING, &TRANSFORM_FEEDBACK_BUFFER_BINDING);
    qDebug() << "GL_TRANSFORM_FEEDBACK_BUFFER_BINDING =" << TRANSFORM_FEEDBACK_BUFFER_BINDING;

    GLint TRANSFORM_FEEDBACK_BUFFER_START;
    glGetIntegerv(GL_TRANSFORM_FEEDBACK_BUFFER_START, &TRANSFORM_FEEDBACK_BUFFER_START);
    qDebug() << "GL_TRANSFORM_FEEDBACK_BUFFER_START =" << TRANSFORM_FEEDBACK_BUFFER_START;

    GLint TRANSFORM_FEEDBACK_BUFFER_SIZE;
    glGetIntegerv(GL_TRANSFORM_FEEDBACK_BUFFER_SIZE, &TRANSFORM_FEEDBACK_BUFFER_SIZE);
    qDebug() << "GL_TRANSFORM_FEEDBACK_BUFFER_SIZE =" << TRANSFORM_FEEDBACK_BUFFER_SIZE;

    GLint UNIFORM_BUFFER_BINDING;
    glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &UNIFORM_BUFFER_BINDING);
    qDebug() << "GL_UNIFORM_BUFFER_BINDING =" << UNIFORM_BUFFER_BINDING;

    GLint UNIFORM_BUFFER_OFFSET_ALIGNMENT;
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &UNIFORM_BUFFER_OFFSET_ALIGNMENT);
    qDebug() << "GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT =" << UNIFORM_BUFFER_OFFSET_ALIGNMENT;

    GLint UNIFORM_BUFFER_SIZE;
    glGetIntegerv(GL_UNIFORM_BUFFER_SIZE, &UNIFORM_BUFFER_SIZE);
    qDebug() << "GL_UNIFORM_BUFFER_SIZE =" << UNIFORM_BUFFER_SIZE;

    GLint UNIFORM_BUFFER_START;
    glGetIntegerv(GL_UNIFORM_BUFFER_START, &UNIFORM_BUFFER_START);
    qDebug() << "GL_UNIFORM_BUFFER_START =" << UNIFORM_BUFFER_START;

    GLfloat UNPACK_ALIGNMENT;
    glGetFloatv(GL_UNPACK_ALIGNMENT, &UNPACK_ALIGNMENT);
    qDebug() << "GL_UNPACK_ALIGNMENT =" << UNPACK_ALIGNMENT;

    GLfloat UNPACK_IMAGE_HEIGHT;
    glGetFloatv(GL_UNPACK_IMAGE_HEIGHT, &UNPACK_IMAGE_HEIGHT);
    qDebug() << "GL_UNPACK_IMAGE_HEIGHT =" << UNPACK_IMAGE_HEIGHT;

    GLboolean UNPACK_LSB_FIRST;
    glGetBooleanv(GL_UNPACK_LSB_FIRST, &UNPACK_LSB_FIRST);
    qDebug() << "GL_UNPACK_LSB_FIRST =" << (UNPACK_LSB_FIRST ? "true" : "false");

    GLfloat UNPACK_ROW_LENGTH;
    glGetFloatv(GL_UNPACK_ROW_LENGTH, &UNPACK_ROW_LENGTH);
    qDebug() << "GL_UNPACK_ROW_LENGTH =" << UNPACK_ROW_LENGTH;

    GLfloat UNPACK_SKIP_IMAGES;
    glGetFloatv(GL_UNPACK_SKIP_IMAGES, &UNPACK_SKIP_IMAGES);
    qDebug() << "GL_UNPACK_SKIP_IMAGES =" << UNPACK_SKIP_IMAGES;

    GLfloat UNPACK_SKIP_PIXELS;
    glGetFloatv(GL_UNPACK_SKIP_PIXELS, &UNPACK_SKIP_PIXELS);
    qDebug() << "GL_UNPACK_SKIP_PIXELS =" << UNPACK_SKIP_PIXELS;

    GLfloat UNPACK_SKIP_ROWS;
    glGetFloatv(GL_UNPACK_SKIP_ROWS, &UNPACK_SKIP_ROWS);
    qDebug() << "GL_UNPACK_SKIP_ROWS =" << UNPACK_SKIP_ROWS;

    GLboolean UNPACK_SWAP_BYTES;
    glGetBooleanv(GL_UNPACK_SWAP_BYTES, &UNPACK_SWAP_BYTES);
    qDebug() << "GL_UNPACK_SWAP_BYTES =" << (UNPACK_SWAP_BYTES ? "true" : "false");

    GLboolean VERTEX_PROGRAM_POINT_SIZE;
    glGetBooleanv(GL_VERTEX_PROGRAM_POINT_SIZE, &VERTEX_PROGRAM_POINT_SIZE);
    qDebug() << "GL_VERTEX_PROGRAM_POINT_SIZE =" << (VERTEX_PROGRAM_POINT_SIZE ? "true" : "false");

    GLint VIEWPORT[4];
    glGetIntegerv(GL_VIEWPORT, &VIEWPORT[0]);
    qDebug() << "GL_VIEWPORT = Rect(" << VIEWPORT[0] << VIEWPORT[1] << VIEWPORT[2] << VIEWPORT[3] << ")";

    GLint FRAMEBUFFER_BINDING;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &FRAMEBUFFER_BINDING);
    qDebug() << "GL_FRAMEBUFFER_BINDING =" << FRAMEBUFFER_BINDING;


    qDebug() << "============";
}

void OgreNode::saveOgreState()
{
    m_ogreFBO = getOgreFBO();

    const QGLContext *ctx = QGLContext::currentContext();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    ctx->functions()->glBindBuffer(GL_ARRAY_BUFFER, 0);
    ctx->functions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    ctx->functions()->glBindRenderbuffer(GL_RENDERBUFFER, 0);
    ctx->functions()->glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
}

void OgreNode::restoreOgreState()
{
    const QGLContext *ctx = QGLContext::currentContext();
    glPopAttrib();
    ctx->functions()->glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_ogreFBO);
    ctx->functions()->glUseProgram(0);
}

GLuint OgreNode::getOgreFBO()
{
    if (!m_renderTexture)
        return 0;

    Ogre::GLFrameBufferObject *ogreFbo = 0;
    m_renderTexture->getCustomAttribute("FBO", &ogreFbo);
    Ogre::GLFBOManager *manager = ogreFbo->getManager();
    manager->bind(m_renderTexture);

    GLint id;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &id);

    const QGLContext *ctx = QGLContext::currentContext();
    ctx->functions()->glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);

    return id;
}

void OgreNode::preprocess()
{
    restoreOgreState();
    m_renderTexture->update(true);
    saveOgreState();
}

void OgreNode::update()
{
    restoreOgreState();

    if (!m_initialized)
        init();

    if (m_dirtyFBO)
        updateFBO();

    saveOgreState();
}

void OgreNode::updateFBO()
{
    if (m_renderTexture)
        Ogre::TextureManager::getSingleton().remove("RttTex");

    rtt_texture = Ogre::TextureManager::getSingleton().createManual("RttTex",
                                                                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                                    Ogre::TEX_TYPE_2D,
                                                                    m_size.width(),
                                                                    m_size.height(),
                                                                    0,
                                                                    Ogre::PF_R8G8B8A8,
                                                                    Ogre::TU_RENDERTARGET, 0, false,
                                                                    m_AAEnabled ? m_samples : 0);

    m_renderTexture = rtt_texture->getBuffer()->getRenderTarget();

    m_renderTexture->addViewport(m_camera);
    m_renderTexture->getViewport(0)->setClearEveryFrame(true);
    m_renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
    m_renderTexture->getViewport(0)->setOverlaysEnabled(false);

    Ogre::Real aspectRatio = Ogre::Real(m_size.width()) / Ogre::Real(m_size.height());
    m_camera->setAspectRatio(aspectRatio);

    QSGGeometry::updateTexturedRectGeometry(&m_geometry,
                                            QRectF(0, 0, m_size.width(), m_size.height()),
                                            QRectF(0, 0, 1, 1));

    Ogre::GLTexture *nativeTexture = static_cast<Ogre::GLTexture *>(rtt_texture.get());

    delete m_texture;
    m_texture = new QSGPlainTexture;
    m_texture->setOwnsTexture(false);
    m_texture->setTextureSize(m_size);
    m_texture->setTextureId(nativeTexture->getGLID());

    m_material.setTexture(m_texture);
    m_materialO.setTexture(m_texture);
}

void OgreNode::setSize(const QSize &size)
{
    if (size == m_size)
        return;

    m_size = size;
    m_dirtyFBO = true;
    markDirty(DirtyGeometry);
}

void OgreNode::setAAEnabled(bool enable)
{
    if (m_AAEnabled == enable)
        return;

    m_AAEnabled = enable;
    m_dirtyFBO = true;
    markDirty(DirtyMaterial);
}

void OgreNode::init()
{
    const QGLContext *ctx = QGLContext::currentContext();
    QGLFormat format = ctx->format();
    m_samples = format.sampleBuffers() ? format.samples() : 0;

    m_root = new Ogre::Root;
    m_root->loadPlugin(Ogre::String(OGRE_PLUGIN_DIR) + "/RenderSystem_GL");

    Ogre::RenderSystem *renderSystem = m_root->getRenderSystemByName("OpenGL Rendering Subsystem");
    m_root->setRenderSystem(renderSystem);
    m_root->initialise(false);

    Ogre::NameValuePairList params;

    params["externalGLControl"] = "true";
    params["currentGLContext"] = "true";

    //Finally create our window.
    m_window = m_root->createRenderWindow("OgreWindow", 1, 1, false, &params);
    m_window->setVisible(false);
    m_window->update(false);

    // Load resources
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(QString(appPath() + "/resources/data.zip").toAscii().data(), "Zip");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Setup scene
    m_sceneManager = m_root->createSceneManager(Ogre::ST_GENERIC, "mySceneManager");
    m_camera = m_sceneManager->createCamera("myCamera");
    m_camera->setNearClipDistance(1);
    m_camera->setFarClipDistance(99999);
    m_camera->setAspectRatio(Ogre::Real(m_size.width()) / Ogre::Real(m_size.height()));

    // Setup content...

    // Set a sky dome
    m_sceneManager->setSkyBox(true, "SpaceSkyBox", 10000);

    // setup some basic lighting for our scene
    m_sceneManager->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));
    m_sceneManager->createLight("myLight")->setPosition(20, 80, 50);

    // create an ogre head entity and place it at the origin
    m_sceneManager->getRootSceneNode()->attachObject(m_sceneManager->createEntity("Head", "ogrehead.mesh"));

    // Setup the camera
    m_cameraObject = new CameraNodeObject(m_camera);
    m_cameraObject->camera()->setAutoTracking(true, m_sceneManager->getRootSceneNode());

    m_initialized = true;
}
