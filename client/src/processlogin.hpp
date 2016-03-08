/*
 * =====================================================================================
 *
 *       Filename: processlogin.hpp
 *        Created: 8/14/2015 2:47:30 PM
 *  Last Modified: 03/07/2016 23:47:54
 *
 *    Description: 
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */
#pragma once

#include <cstdint>
#include <SDL2/SDL.h>
#include "process.hpp"
#include "tokenboard.hpp"
#include "inputbox.hpp"
#include "passwordbox.hpp"
#include "message.hpp"
#include "button.hpp"

class ProcessLogin: public Process
{
    private:
        // GUI part
        // naked texture ID's
        uint32_t        m_TextureBackground1;
        uint32_t        m_TextureBackground2;
        uint32_t        m_FrameBox;

        Button          m_Button1;
        Button          m_Button2;
        Button          m_Button3;
        Button          m_Button4;

        InputBox        m_IDInputBox;
        PasswordBox     m_PasswordBox;

    public:
        ProcessLogin(Game *);
        ~ProcessLogin();

    public:
        void Enter();
        void Exit();
        void Update();
        void Draw();
        void HandleEvent(SDL_Event *);

    private:
        void DoLogin();
};
