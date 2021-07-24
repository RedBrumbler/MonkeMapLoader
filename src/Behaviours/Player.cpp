#include "Behaviours/Player.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils-fields.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Rigidbody.hpp"
#include "UnityEngine/CapsuleCollider.hpp"
#include "UnityEngine/SphereCollider.hpp"

#include "GorillaLocomotion/Player.hpp"

DEFINE_TYPE(MapLoader, Player);

extern Logger& getLogger();

using namespace UnityEngine;
using namespace GorillaLocomotion;

namespace MapLoader
{
    void Player::Start()
    {
        instance = this;

        Transform* parent = get_transform();
        
        std::string path = "";
        while (parent)
        {
            Il2CppString* nameCS = parent->get_gameObject()->get_name();    
            std::string name = to_utf8(csstrtostr(nameCS));

            path = string_format("%s/%s", name.c_str(), path.c_str());
            parent = parent->get_parent();
        }

        getLogger().info("Path: %s", path.c_str());
    }

    void Player::TeleportPlayer(Transform* destination)
    {
        if (instance->isTeleporting)
            return;
        getLogger().info("Teleporting Player!");

        GameObject* destGO = destination->get_gameObject();
        Il2CppString* goName = destGO->get_name();
        std::string name = to_utf8(csstrtostr(goName));

        getLogger().info("Teleporting to Object %s", name.c_str());

        instance->isTeleporting = true;
        instance->destination = destination;

        GameObject* go = instance->get_gameObject();
        Rigidbody* playerRigidBody = go->GetComponent<Rigidbody*>();;

        if (playerRigidBody)
        { 
            auto* player = go->GetComponent<GorillaLocomotion::Player*>();

            playerRigidBody->set_velocity(Vector3::get_zero());
            playerRigidBody->set_isKinematic(true);

            Transform* playerTransform = player->get_transform();
            CapsuleCollider* bodyCollider = player->bodyCollider;

            Transform* colliderTransform = bodyCollider->get_transform();
            Vector3 pos = destination->get_position();
            Vector3 playerPos = playerTransform->get_position();
            Vector3 colliderPos = colliderTransform->get_position();

            float x = pos.x - colliderPos.x + playerPos.x;
            float y = pos.y - colliderPos.y + playerPos.y;
            float z = pos.z - colliderPos.z + playerPos.z;

            Vector3 correctedPosition = {x, y, z};

            getLogger().info("dest: %.2f, %.2f, %.2f", correctedPosition.x, correctedPosition.y, correctedPosition.z);
            playerTransform->set_position(correctedPosition);
            
            Quaternion destOrig = destination->get_rotation();
            Vector3 destEuler = destOrig.get_eulerAngles();

            SphereCollider* headCollider = player->headCollider;
            Transform* headTransform = headCollider->get_transform();
            Quaternion headRot = headTransform->get_rotation();
            Vector3 headEuler = headRot.get_eulerAngles();
            float turnAmt = destEuler.y - headEuler.y;
            player->Turn(turnAmt);

            // set last position
            player->lastPosition = correctedPosition;

            // reset velocity history
            Array<Vector3>* velocityHistory = reinterpret_cast<Array<Vector3>*>(il2cpp_functions::array_new(classof(Vector3), player->velocityHistorySize));
            player->velocityHistory = velocityHistory;

            // set last head pos
            Vector3 headPos = headTransform->get_position();
            player->lastHeadPosition = headPos;

            // set last left hand pos
            Vector3 lastLeftHandPos = player->CurrentLeftHandPosition();
            player->lastLeftHandPosition = lastLeftHandPos;

            // set last right hand pos
            Vector3 lastRightHandPos = player->CurrentRightHandPosition();
            player->lastRightHandPosition = lastRightHandPos;

            // set velocities to zero
            player->currentVelocity = Vector3::get_zero();
            player->denormalizedVelocityAverage = Vector3::get_zero();

            // we done pog
            playerRigidBody->set_isKinematic(false);
        }
        instance->isTeleporting = false;
    }

    void Player::Offset(Vector3 offset)
    {
        GameObject* go = instance->get_gameObject();
        Rigidbody* playerRigidBody = go->GetComponent<Rigidbody*>();;

        if (playerRigidBody)
        { 
            auto* player = go->GetComponent<GorillaLocomotion::Player*>();

            Transform* playerTransform = player->get_transform();
            CapsuleCollider* bodyCollider = player->bodyCollider;


            Vector3 oldPos = playerTransform->get_position();
            Vector3 pos = oldPos + offset;
            
            playerTransform->set_position(pos);

            
            SphereCollider* headCollider = player->headCollider;
            Transform* headTransform = headCollider->get_transform();

            // set last position
            player->lastPosition = oldPos;

            // set last head pos
            Vector3 headPos = headTransform->get_position();
            player->lastHeadPosition = headPos;

            // set last left hand pos
            Vector3 lastLeftHandPos = player->CurrentLeftHandPosition();
            player->lastLeftHandPosition = lastLeftHandPos;

            // set last right hand pos
            Vector3 lastRightHandPos = player->CurrentRightHandPosition();
            player->lastRightHandPosition = lastRightHandPos;
        }
    }

    Vector3 Player::get_localizedPosition()
    {
        GameObject* go = instance->get_gameObject();
        auto* player = go->GetComponent<GorillaLocomotion::Player*>();

        CapsuleCollider* bodyCollider = player->bodyCollider;

        Transform* colliderTransform = bodyCollider->get_transform();
        Vector3 colliderPos = colliderTransform->get_position();

        return colliderPos;
    }

    Player* Player::get_instance()
    {
        return instance;
    }
}