

#ifndef __CCCAImageCube_H__
#define __CCCAImageCube_H__

#include "images/CAImage.h"

#include <string>
#include <unordered_map>
#include "ccTypes.h"

NS_CC_BEGIN

/**
 * @addtogroup _3d
 * @{
 */

/**
 CAImageCube is a collection of six separate square textures that are put 
 onto the faces of an imaginary cube.
*/
class CC_DLL CAImageCube: public CAImage
{
public:
    /** create cube texture from 6 textures.
       @param positive_x texture for the right side of the texture cube face.
       @param negative_x texture for the up side of the texture cube face.
       @param positive_y texture for the top side of the texture cube face
       @param negative_y texture for the bottom side of the texture cube face
       @param positive_z texture for the forward side of the texture cube face.
       @param negative_z texture for the rear side of the texture cube face.
       @return  A new texture cube inited with given parameters.
    */
    static CAImageCube* create(const std::string& positive_x, const std::string& negative_x,
                               const std::string& positive_y, const std::string& negative_y,
                               const std::string& positive_z, const std::string& negative_z);

    /** Sets the min filter, mag filter, wrap s and wrap t texture parameters.
    If the texture size is NPOT (non power of 2), then in can only use GL_CLAMP_TO_EDGE in GL_TEXTURE_WRAP_{S,T}.
    */
    void setTexParameters(const TexParams&);

    /** reload texture cube after GLESContext reconstructed.*/
    bool reloadImage();

    /**
    * Constructor.
    */
    CAImageCube();

    /**
    * Destructor.
    */
    virtual ~CAImageCube();
protected:

    bool init(const std::string& positive_x, const std::string& negative_x,
              const std::string& positive_y, const std::string& negative_y,
              const std::string& positive_z, const std::string& negative_z);
private:
    std::vector<std::string> _imgPath;
};

// end of 3d group
/// @}

NS_CC_END

#endif // __CCCAImageCube_H__
