// Copyright 2013 The Chromium Authors. All rights reserved.
// Copyright 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ozone/wayland/window.h"

#include "ozone/wayland/display.h"
#include "ozone/wayland/egl/egl_window.h"
#include "ozone/wayland/shell_surface.h"
#include "ozone/wayland/surface.h"

#include "base/logging.h"

namespace ozonewayland {

WaylandWindow::WaylandWindow(unsigned handle) :
    shell_surface_(NULL),
    handle_(handle),
    window_(NULL),
    type_(None),
    allocation_(gfx::Rect(0, 0, 1, 1))
{
}

WaylandWindow::~WaylandWindow() {
  if (window_) {
    delete window_;
    window_ = NULL;
  }

  if (shell_surface_)
  {
    delete shell_surface_;
    shell_surface_ = NULL;
  }
}

void WaylandWindow::SetShellType(ShellType type)
{
  if (type_ == type)
    return;

  if (!shell_surface_)
    shell_surface_ = new WaylandShellSurface(this);

  type_ = type;
  switch (type_) {
    case TOPLEVEL:
      shell_surface_->UpdateShellSurface(TOPLEVEL);
      break;
    case MENU:
      shell_surface_->UpdateShellSurface(MENU);
      break;
    case FULLSCREEN:
    case TRANSIENT:
    case CUSTOM:
      NOTREACHED() << "UnSupported Shell Type.";
      break;
    default:
      break;
  }
}

void WaylandWindow::SetWindowTitle(const string16& title) {
  shell_surface_->SetWindowTitle(title);
}

void WaylandWindow::Maximize()
{
  NOTIMPLEMENTED();
}

void WaylandWindow::Minimize()
{
  NOTIMPLEMENTED();
}

void WaylandWindow::Restore()
{
  NOTIMPLEMENTED();
}

void WaylandWindow::SetFullscreen()
{
  NOTIMPLEMENTED();
}

void WaylandWindow::RealizeAcceleratedWidget()
{
  if (!window_)
    window_ = new EGLWindow(shell_surface_->Surface()->wlSurface(),
                            allocation_.width(), allocation_.height());
}

void WaylandWindow::HandleSwapBuffers()
{
  shell_surface_->Surface()->EnsureFrameCallBackDone();
  shell_surface_->Surface()->AddFrameCallBack();
}

wl_egl_window* WaylandWindow::egl_window() const
{
  return window_ ? window_->egl_window() : 0;
}

struct wl_surface* WaylandWindow::GetSurface() const {
  return shell_surface_ ? shell_surface_->Surface()->wlSurface() : 0;
}

bool WaylandWindow::SetBounds(const gfx::Rect& new_bounds)
{
  int width = new_bounds.width();
  int height = new_bounds.height();
  allocation_ = gfx::Rect(allocation_.x(), allocation_.y(), width, height);
  if (!shell_surface_ || !window_)
      return false;

  return window_->Resize(shell_surface_->Surface(), width, height);
}

}  // namespace ozonewayland
