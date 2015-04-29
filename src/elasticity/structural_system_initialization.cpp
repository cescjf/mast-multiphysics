/*
 * MAST: Multidisciplinary-design Adaptation and Sensitivity Toolkit
 * Copyright (C) 2013-2015  Manav Bhatia
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


// MAST includes
#include "elasticity/structural_system_initialization.h"


MAST::StructuralSystemInitialization::
StructuralSystemInitialization(libMesh::System& sys,
                               const std::string& prefix,
                               const libMesh::FEType& fe_type):
MAST::SystemInitialization(sys, prefix) {
    
    _vars.resize(6);
    
    std::string nm = prefix + "_ux";
    _vars[0] = sys.add_variable(nm, fe_type);

    nm = prefix + "_uy";
    _vars[1] = sys.add_variable(nm, fe_type);

    nm = prefix + "_uz";
    _vars[2] = sys.add_variable(nm, fe_type);

    nm = prefix + "_tx";
    _vars[3] = sys.add_variable(nm, fe_type);

    nm = prefix + "_ty";
    _vars[4] = sys.add_variable(nm, fe_type);

    nm = prefix + "_tz";
    _vars[5] = sys.add_variable(nm, fe_type);

}


MAST::StructuralSystemInitialization::~StructuralSystemInitialization() {
    
}
