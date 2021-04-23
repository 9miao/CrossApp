
#include "renderer/CCGroupCommand.h"
#include "renderer/CCRenderer.h"
#include "basics/CAApplication.h"

NS_CC_BEGIN

GroupCommandManager::GroupCommandManager()
{

}

GroupCommandManager::~GroupCommandManager()
{
    
}

bool GroupCommandManager::init()
{
    //0 is the default render group
    _groupMapping[0] = true;
    return true;
}

int GroupCommandManager::getGroupID()
{
    //Reuse old id
    if (!_unusedIDs.empty())
    {
        int groupID = *_unusedIDs.rbegin();
        _unusedIDs.pop_back();
        _groupMapping[groupID] = true;
        return groupID;
    }

    //Create new ID
//    int newID = _groupMapping.size();
    int newID = CAApplication::getApplication()->getRenderer()->createRenderQueue();
    _groupMapping[newID] = true;

    return newID;
}

void GroupCommandManager::releaseGroupID(int groupID)
{
    _groupMapping[groupID] = false;
    _unusedIDs.push_back(groupID);
}

GroupCommand::GroupCommand()
{
    _type = RenderCommand::Type::GROUP_COMMAND;
    _renderQueueID = CAApplication::getApplication()->getRenderer()->getGroupCommandManager()->getGroupID();
}

void GroupCommand::init(float globalOrder)
{
    _zOrder = globalOrder;
    auto manager = CAApplication::getApplication()->getRenderer()->getGroupCommandManager();
    manager->releaseGroupID(_renderQueueID);
    _renderQueueID = manager->getGroupID();
}

GroupCommand::~GroupCommand()
{
    CAApplication::getApplication()->getRenderer()->getGroupCommandManager()->releaseGroupID(_renderQueueID);
}

NS_CC_END
