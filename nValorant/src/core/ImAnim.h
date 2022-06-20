#pragma once
#include <unordered_map>

// ImGui includes
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>


/*
     MIT License

     Copyright (c) 2022 ImAnim

     Permission is hereby granted, free of charge, to any person obtaining a copy
     of this software and associated documentation files (the "Software"), to deal
     in the Software without restriction, including without limitation the rights
     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
     copies of the Software, and to permit persons to whom the Software is
     furnished to do so, subject to the following conditions:

     The above copyright notice and this permission notice shall be included in all
     copies or substantial portions of the Software.

     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
     SOFTWARE.
*/

namespace ImAnim
{
    // A unique ID used by widgets (typically the result of hashing a stack of string)
    typedef unsigned int ImGuiID;

    // Normal animator, hence it changes one float to an another within time
    class Animator
    {
    public:
        Animator(std::unordered_map<ImGuiID, float>* arrValues, ImGuiID id, float flFrom, float flTo, float flSpeed, float flDeltaTime)
        {
            this->arrValues = arrValues;
            this->flFrom = flFrom;
            this->flTo = flTo;
            this->flSpeed = flSpeed;
            this->flDeltaTime = flDeltaTime;
        }

        // This function will convert the flFrom to flTo within time with the choosen speed
        // Is used for when e.g hovered
        void Forward()
        {
            auto currValue = arrValues->find(id);
            if (currValue == arrValues->end())
            {
                arrValues->insert({ id, flFrom });
                currValue = arrValues->find(id);
            }

            currValue->second = ImClamp(currValue->second + flSpeed * flDeltaTime, flFrom, flTo);
        }

        // This function will convert the flTo to FlFrom within time with the choosen speed
        // Is used for when e.g no longer hovered
        void Reverse()
        {
            auto currValue = arrValues->find(id);
            if (currValue == arrValues->end())
            {
                arrValues->insert({ id, flFrom });
                currValue = arrValues->find(id);
            }

            currValue->second = ImClamp(currValue->second - flSpeed * flDeltaTime, flFrom, flTo);
        }

        // You have to call this everytime!
        constexpr void SetDeltaTime(float flDeltaTime)
        {
            this->flDeltaTime = flDeltaTime;
        }

        // You can get the value manuelly by your map, but this makes your code cleaner!
        float GetValue() const
        {
            return arrValues->find(id)->second;
        }

    private:
        // the unordered_map, we are using a map 
        // so you can create one animator for each element!
        // We are not using regular map as we are not trying to 
        std::unordered_map<ImGuiID, float>* arrValues;

        // This is the most important part, without this we wont be able to know which animation is for which e.g button
        ImGuiID id = NULL;

        // The values required to do this animation
        float flFrom = 0.0f;
        float flTo = 0.0f;
        float flSpeed = 0.0f;
        float flDeltaTime = 0.0f;
    };

    class ColorAnimator
    {
    public:
        ColorAnimator(std::unordered_map<ImGuiID, ImVec4>* arrValues, ImGuiID id, ImVec4 flFrom, ImVec4 flTo, float flSpeed, float flDeltaTime)
        {
            this->arrValues = arrValues;
            this->flFrom = flFrom;
            this->flTo = flTo;
            this->flSpeed = flSpeed;
            this->flDeltaTime = flDeltaTime;
        }

        // This function will convert the flFrom to flTo within time with the choosen speed
        // Is used for when e.g hovered
        void Forward()
        {
            auto currValue = arrValues->find(id);
            if (currValue == arrValues->end())
            {
                arrValues->insert({ id, flFrom });
                currValue = arrValues->find(id);
            }

            currValue->second = Transition(currValue->second, flTo);
        }

        // This function will convert the flTo to FlFrom within time with the choosen speed
        // Is used for when e.g no longer hovered
        void Reverse()
        {
            auto currValue = arrValues->find(id);
            if (currValue == arrValues->end())
            {
                arrValues->insert({ id, flFrom });
                currValue = arrValues->find(id);
            }

            currValue->second = Transition(flTo, currValue->second);
        }

        ImVec4 GetValue() const
        {
            return arrValues->find(id)->second;
        }

    private:
        // the unordered_map, we are using a map 
        // so you can create one animator for each element!
        // We are not using regular map as we are not trying to 
        std::unordered_map<ImGuiID, ImVec4>* arrValues;

        // This is the most important part, without this we wont be able to know which animation is for which e.g button
        ImGuiID id = NULL;

        // The values required to do this animation
        ImVec4 flFrom = ImVec4();
        ImVec4 flTo = ImVec4();
        float flSpeed = 0.0f;
        float flDeltaTime = 0.0f;

        // Can be done better, might need a recode
        ImVec4 Transition(ImVec4 from, ImVec4 to)
        {
            const float flTime = flSpeed * flDeltaTime;

            if (from.x <= to.x)
            {
                from.x = ImMin(from.x + flTime, to.x);
            }
            else if (from.x >= to.x)
            {
                from.x = ImMax(to.x, from.x - flTime);
            }

            if (from.y <= to.y)
            {
                from.y = ImMin(from.y + flTime, to.y);
            }
            else if (from.y >= to.y)
            {
                from.y = ImMax(to.y, from.y - flTime);
            }

            if (from.z <= flTo.z)
            {
                from.z = ImMin(from.z + flTime, to.z);
            }
            else if (from.z >= to.z)
            {
                from.z = ImMax(to.z, from.z - flTime);
            }

            return from;
        }
    };
}
