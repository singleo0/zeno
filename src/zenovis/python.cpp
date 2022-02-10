#if 0
#include "stdafx.hpp"
#include "main.hpp"
#include "zenvis.h"

#ifndef ZENVIS_LIB

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

PYBIND11_MODULE(pylib_zenvis, m) {
    m.def("initialize", zenvis::initialize);
    m.def("finalize", zenvis::finalize);
    m.def("new_frame", zenvis::new_frame);
    m.def("set_window_size", zenvis::set_window_size);
    m.def("set_curr_frameid", zenvis::set_curr_frameid);
    m.def("get_curr_frameid", zenvis::get_curr_frameid);
    m.def("get_solver_interval", zenvis::get_solver_interval);
    m.def("get_render_fps", zenvis::get_render_fps);
    m.def("look_perspective", zenvis::look_perspective);
    m.def("set_perspective", zenvis::set_perspective);
    m.def("clear_graphics", zenvis::clear_graphics);
    m.def("auto_gc_frame_data", zenvis::auto_gc_frame_data);
    m.def("get_valid_frames_list", zenvis::get_valid_frames_list);
    m.def("load_file", zenvis::load_file);
    m.def("do_screenshot", zenvis::do_screenshot);
    m.def("set_show_grid", zenvis::set_show_grid);
    m.def("new_frame_offline", zenvis::new_frame_offline);
    m.def("set_background_color", zenvis::set_background_color);
    m.def("get_background_color", zenvis::get_background_color);
    m.def("set_smooth_shading", zenvis::set_smooth_shading);
    m.def("set_render_wireframe", zenvis::set_render_wireframe);
}

#endif
/****\

server -> client: (per-frame)

{frameid}:{solver_frameid}:{solver_interval}:{jpegData}

client -> server: (per-mouse-event)

{nx}:{ny}:{cx}:{cy}:{cz}:{theta}:{phi}:{radius}:{fov}:{ortho_mode}:{set_frameid}

^^^NVM: our web enginneer never get hired in to handle zenwebvis..^^^

\****/
#endif