
#ifndef _CC_GROUPCOMMAND_H_
#define _CC_GROUPCOMMAND_H_

#include <vector>
#include <unordered_map>

#include "basics/CAObject.h"
#include "renderer/CCRenderCommand.h"

/**
 * @addtogroup renderer
 * @{
 */

NS_CC_BEGIN

//Used for internal
class GroupCommandManager : public CAObject
{
public:
    int getGroupID();
    void releaseGroupID(int groupID);

protected:
    friend class Renderer;
    GroupCommandManager();
    ~GroupCommandManager();
    bool init();
    std::unordered_map<int, bool> _groupMapping;
    std::vector<int> _unusedIDs;
};

/**
 GroupCommand is used to group several command together, and more, it can be nested.
 So it is used to generate the hierarchy for the rendcommands. Every group command will be assigned by a group ID.
 */
class CC_DLL GroupCommand : public RenderCommand
{
public:
    /**@{
     Constructor and Destructor.
     */
    GroupCommand();
    ~GroupCommand();
    /**@}*/
    
    /**Init function for group command*/
    void init(float globalOrder);
    
    /**called by renderer, get the group ID.*/
    inline int getRenderQueueID() const {return _renderQueueID;}
    
protected:
    int _renderQueueID;
};

NS_CC_END

/**
 end of support group
 @}
 */
#endif //_CC_GROUPCOMMAND_H_
