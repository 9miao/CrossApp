

#ifndef __CCGLPROGRAMCACHE_H__
#define __CCGLPROGRAMCACHE_H__

#include "renderer/CCGLProgram.h"

/**
 * @addtogroup renderer
 * @{
 */

NS_CC_BEGIN

/** GLProgramCache
 Singleton that stores manages GLProgram objects (shaders)
 @since v2.0
 */
class CC_DLL GLProgramCache : public CAObject
{
public:
    /**
    Constructor.
     * @js ctor
     */
    GLProgramCache();
    /**
    Destructor.
     * @js NA
     * @lua NA
     */
    ~GLProgramCache();

    /** returns the shared instance */
    static GLProgramCache* getInstance();

    /** purges the cache. It releases the retained instance. */
    static void destroyInstance();

    /** loads the default shaders */
    void loadDefaultGLPrograms();

    /** reload the default shaders */
    void reloadDefaultGLPrograms();

    /** returns a GL program for a given key 
     */
    GLProgram * getGLProgram(const std::string &key);

    /** adds a GLProgram to the cache for a given name */
    void addGLProgram(GLProgram* program, const std::string &key);
    
    /** reload default programs these are relative to light */
    void reloadDefaultGLProgramsRelativeToLights();

private:
    /**
    @{
        Init and load predefined shaders.
    */
    bool init();
    void loadDefaultGLProgram(GLProgram *program, int type);
    /**
    @}
    */
    /**Predefined shaders.*/
    std::unordered_map<std::string, GLProgram*> _programs;
};

NS_CC_END
// end of shaders group
/// @}

#endif /* __CCGLPROGRAMCACHE_H__ */
