#pragma once

class Node {
public:
    virtual ~Node() = default;

    virtual void Update() {}
    virtual void Render() {}
};
