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

#ifndef __mast__second_order_newmark_transient_solver__
#define __mast__second_order_newmark_transient_solver__

// MAST includes
#include "solver/transient_solver_base.h"


namespace MAST {
    
    
    /*!
     *    This class implements the Newmark solver for solution of a
     *    second-order ODE.
     *
     */
    class SecondOrderNewmarkTransientSolver:
    public MAST::TransientSolverBase {
    public:
        SecondOrderNewmarkTransientSolver();
        
        virtual ~SecondOrderNewmarkTransientSolver();
        
        /*!
         *    \f$ \beta \f$ parameter used by this solver.
         */
        Real beta;

        /*!
         *    \f$ \gamma \f$ parameter used by this solver.
         */
        Real gamma;

        /*!
         *    @returns the highest order time derivative that the solver
         *    will handle
         */
        virtual int ode_order() const {
            return 2;
        }

        /*!
         *   solves the current time step for solution and velocity
         */
        virtual void solve();
        
        /*!
         *   advances the time step and copies the current solution to old
         *   solution, and so on.
         */
        virtual void advance_time_step();
        
    protected:
        
        /*!
         *    @returns the number of iterations for which solution and velocity
         *    are to be stored.
         */
        virtual unsigned int _n_iters_to_store() const {
            return 2;
        }
        
        /*!
         *    provides the element with the transient data for calculations
         */
        virtual void _set_element_data(std::vector<libMesh::dof_id_type>& dof_indices,
                                       MAST::ElementBase& elem);
        
        /*!
         *    update the transient velocity based on the current solution
         */
        virtual void _update_velocity(libMesh::NumericVector<Real>& vec);
        
        /*!
         *    update the transient acceleration based on the current solution
         */
        virtual void _update_acceleration(libMesh::NumericVector<Real>& vec);
        
        /*!
         *   performs the element calculations over \par elem, and returns
         *   the element vector and matrix quantities in \par mat and
         *   \par vec, respectively. \par if_jac tells the method to also
         *   assemble the Jacobian, in addition to the residual vector.
         */
        virtual void
        _elem_calculations(MAST::ElementBase& elem,
                           const std::vector<libMesh::dof_id_type>& dof_indices,
                           bool if_jac,
                           RealVectorX& vec,
                           RealMatrixX& mat);
        
        /*!
         *   performs the element sensitivity calculations over \par elem,
         *   and returns the element residual sensitivity in \par vec .
         */
        virtual void
        _elem_sensitivity_calculations(MAST::ElementBase& elem,
                                       const std::vector<libMesh::dof_id_type>& dof_indices,
                                       RealVectorX& vec);
    };
    
}

#endif // __mast__second_order_newmark_transient_solver__
