

#ifndef __CAShaderCache_H__
#define __CAShaderCache_H__

#include "basics/CASTLContainer.h"

NS_CC_BEGIN

class CAGLProgram;

/**
 * @addtogroup shaders
 * @{
 */

/** CAShaderCache
 Singleton that stores manages GL shaders
 @since v2.0
 */
class CC_DLL CAShaderCache : public CAObject 
{
public:
    /**
     * @js ctor
     */
    CAShaderCache();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~CAShaderCache();
    /** returns the shared instance 
     *  @js getInstance
     */
    static CAShaderCache* sharedShaderCache();

    /** purges the cache. It releases the retained instance. */
    static void purgeSharedShaderCache();

    /** loads the default shaders */
    void loadDefaultShaders();
    
    /** reload the default shaders */
    void reloadDefaultShaders();



    /** returns a GL program for a given key 
     *  @js getProgram
     */
    CAGLProgram * programForKey(const char* key);

    /** adds a CAGLProgram to the cache for a given name */
    void addProgram(CAGLProgram* program, const char* key);

private:
    bool init();
    void loadDefaultShader(CAGLProgram *program, int type);

    CAMap<std::string, CAGLProgram*> m_mPrograms;

};

// end of shaders group
/// @}

NS_CC_END

#endif /* __CAShaderCache_H__ */
