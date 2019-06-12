/****************************************************************************
 Copyright (c) 2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "TiledMapAssembler.hpp"
#include "../NodeProxy.hpp"
#include "../ModelBatcher.hpp"
#include "../RenderFlow.hpp"

RENDERER_BEGIN

TiledMapAssembler::TiledMapAssembler()
{
    
}

TiledMapAssembler::~TiledMapAssembler()
{
    
}

void TiledMapAssembler::updateNodes(std::size_t meshIndex, std::vector<std::string> nodes)
{
    _nodesMap[meshIndex] = nodes;
}

void TiledMapAssembler::clearNodes(std::size_t meshIndex)
{
    _nodesMap.erase(meshIndex);
}

void TiledMapAssembler::handle(NodeProxy *node, ModelBatcher* batcher, Scene* scene)
{
    _node = node;
    _batcher = batcher;
    
    _tileMapWorldMat = _node->getWorldMatrix();
    _tileMapWorldMat.translate(-_moveX, -_moveY, 0);
    Assembler::handle(node, batcher, scene);
}

void TiledMapAssembler::fillBuffers(MeshBuffer* buffer, std::size_t index, const Mat4& worldMat)
{
    auto it = _nodesMap.find(index);
    if (it != _nodesMap.end())
    {
        auto flow = _batcher->getFlow();
        for (auto& childName : it->second) {
            auto child = _node->getChildByName(childName);
            if (child)
            {
                child->enableVisit();
                child->disaleUpdateWorldMatrix();
                child->updateFromJS();
                child->updateMatrix(_tileMapWorldMat);
                flow->visit(child);
                child->enableUpdateWorldMatrix();
                child->disableVisit();
            }
        }
    }
    Assembler::fillBuffers(buffer, index, worldMat);
}

RENDERER_END