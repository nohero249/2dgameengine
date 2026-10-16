#pragma once

#include <bitset>
#include <vector>

const unsigned int MAX_COMPONENTS = 32;

/////////////////////////////////////////////////////////////////////////////////
// Signature
/////////////////////////////////////////////////////////////////////////////////
// We use a bitset (1s and 0s) to keep of which components an entity has,
// ans also belps keep track of which entities a system is interested in.
/////////////////////////////////////////////////////////////////////////////////
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent
{
    protected:
    static int nextId;
};

// Used to assign a unique id to a component type
template <typename T>
class Component: public IComponent
{
    // Returns the unuque id of component<T>
    static int GetId()
    {
        static auto id = nextId++;
        return id;
    }
};

class Entity
{
    private:
        int id;

    public:
        Entity(int id): id(id) {};
        Entity(const Entity& entity) = default;
        int GetId() const;

        Entity& operator =(const Entity& other) = default;
        bool operator ==(const Entity& other) const { return id == other.id; };
        bool operator !=(const Entity& other) const { return id != other.id; };
        bool operator >(const Entity& other) const { return id > other.id; };
        bool operator <(const Entity& other) const { return id < other.id; };
};

/////////////////////////////////////////////////////////////////////////////////
// System
/////////////////////////////////////////////////////////////////////////////////
// The system processes entities that contain a spicific signature
/////////////////////////////////////////////////////////////////////////////////
class System
{
    private:
        Signature componentSignature;
        std::vector<Entity> entities;

    public:
        System() = default;
        ~System() = default;

        void AddEntityToSystem(Entity entity);
        void RemoveEntityFromSystem(Entity entity);
        std::vector<Entity> GetSystemEntities() const;
        const Signature& GetComponentSiganure() const;

        // Defines the component type that entities must have to be considered by the system
        template <typename TComponent> void RequireComponent();
};

class Registry
{

};

template <typename TComponent>
void System::RequireComponent()
{
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}