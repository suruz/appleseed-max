
//
// This source file is part of appleseed.
// Visit https://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2016-2018 Francois Beaune, The appleseedhq Organization
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

// appleseed-max headers.
#include "bump/resource.h"

// 3ds Max headers.
#include "appleseed-max-common/_beginmaxheaders.h"
#include <iparamm2.h>
#include "appleseed-max-common/_endmaxheaders.h"

class BumpParamMapDlgProc
  : public ParamMap2UserDlgProc
{
  public:
    void DeleteThis() override
    {
        delete this;
    }

    INT_PTR DlgProc(
        TimeValue   t,
        IParamMap2* map,
        HWND        hwnd,
        UINT        umsg,
        WPARAM      wparam,
        LPARAM      lparam) override
    {
        switch (umsg)
        {
          case WM_INITDIALOG:
            enable_disable_controls(hwnd);
            return TRUE;

          case WM_COMMAND:
            switch (LOWORD(wparam))
            {
              case IDC_COMBO_BUMP_METHOD:
                switch (HIWORD(wparam))
                {
                  case CBN_SELCHANGE:
                    enable_disable_controls(hwnd);
                    return TRUE;

                  default:
                    return FALSE;
                }

              default:
                return FALSE;
            }

          default:
            return FALSE;
        }
    }

  private:
    void enable_disable_controls(HWND hwnd)
    {
        auto selected = SendMessage(GetDlgItem(hwnd, IDC_COMBO_BUMP_METHOD), CB_GETCURSEL, 0, 0);
        EnableWindow(GetDlgItem(hwnd, IDC_COMBO_BUMP_UP_VECTOR), selected == 1 ? TRUE : FALSE);
    }
};
