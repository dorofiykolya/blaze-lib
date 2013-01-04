//=================================================================================================
/*!
//  \file blaze/math/expressions/DVecDVecCrossExpr.h
//  \brief Header file for the dense vector/dense vector cross product expression
//
//  Copyright (C) 2011 Klaus Iglberger - All Rights Reserved
//
//  This file is part of the Blaze library. This library is free software; you can redistribute
//  it and/or modify it under the terms of the GNU General Public License as published by the
//  Free Software Foundation; either version 3, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
//  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See the GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along with a special
//  exception for linking and compiling against the Blaze library, the so-called "runtime
//  exception"; see the file COPYING. If not, see http://www.gnu.org/licenses/.
*/
//=================================================================================================

#ifndef _BLAZE_MATH_EXPRESSIONS_DVECDVECCROSSEXPR_H_
#define _BLAZE_MATH_EXPRESSIONS_DVECDVECCROSSEXPR_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <stdexcept>
#include <blaze/math/constraints/DenseVector.h>
#include <blaze/math/constraints/TransposeFlag.h>
#include <blaze/math/dense/StaticVector.h>
#include <blaze/math/expressions/Computation.h>
#include <blaze/math/expressions/DenseVector.h>
#include <blaze/math/expressions/Expression.h>
#include <blaze/math/expressions/Forward.h>
#include <blaze/math/traits/CrossTrait.h>
#include <blaze/math/traits/MultExprTrait.h>
#include <blaze/math/traits/SubExprTrait.h>
#include <blaze/math/typetraits/IsComputation.h>
#include <blaze/math/typetraits/IsExpression.h>
#include <blaze/math/typetraits/IsTemporary.h>
#include <blaze/util/Assert.h>
#include <blaze/util/constraints/Reference.h>
#include <blaze/util/SelectType.h>
#include <blaze/util/Types.h>


namespace blaze {

//=================================================================================================
//
//  CLASS DVECDVECCROSSEXPR
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Expression object for dense vector-dense vector cross products.
// \ingroup dense_vector_expression
//
// The DVecDVecCrossExpr class represents the compile time expression for cross products
// between dense vectors.
*/
template< typename VT1    // Type of the left-hand side dense vector
        , typename VT2 >  // Type of the right-hand side dense vector
class DVecDVecCrossExpr : public DenseVector< DVecDVecCrossExpr<VT1,VT2>, false >
                        , private Expression
                        , private Computation
{
 private:
   //**Type definitions****************************************************************************
   typedef typename VT1::ResultType     RT1;  //!< Result type of the left-hand side dense vector expression.
   typedef typename VT2::ResultType     RT2;  //!< Result type of the right-hand side dense vector expression.
   typedef typename VT1::ReturnType     RN1;  //!< Return type of the left-hand side dense vector expression.
   typedef typename VT2::ReturnType     RN2;  //!< Return type of the right-hand side dense vector expression.
   typedef typename VT1::CompositeType  CT1;  //!< Composite type of the left-hand side dense vector expression.
   typedef typename VT2::CompositeType  CT2;  //!< Composite type of the right-hand side dense vector expression.
   typedef typename VT1::ElementType    ET1;  //!< Element type of the left-hand side dense vector expression.
   typedef typename VT2::ElementType    ET2;  //!< Element type of the right-hand side dense vector expression.
   //**********************************************************************************************

   //**Return type evaluation**********************************************************************
   //! Compilation switch for the selection of the subscript operator return type.
   /*! The \a returnExpr compile time constant expression is a compilation switch for the
       selection of the \a ReturnType. If either vector operand returns a temporary vector
       or matrix, \a returnExpr will be set to \a false and the subscript operator will
       return it's result by value. Otherwise \a returnExpr will be set to \a true and
       the subscript operator may return it's result as an expression. */
   enum { returnExpr = !IsTemporary<RN1>::value && !IsTemporary<RN2>::value };

   //! Expression return type for the subscript operator.
   typedef typename SubExprTrait< typename MultExprTrait<RN1,RN2>::Type
                                , typename MultExprTrait<RN1,RN2>::Type >::Type  ExprReturnType;
   //**********************************************************************************************

 public:
   //**Type definitions****************************************************************************
   typedef DVecDVecCrossExpr<VT1,VT2>          This;           //!< Type of this DVecDVecCrossExpr instance.
   typedef typename CrossTrait<RT1,RT2>::Type  ResultType;     //!< Result type for expression template evaluations.
   typedef typename ResultType::TransposeType  TransposeType;  //!< Transpose type for expression template evaluations.
   typedef typename ResultType::ElementType    ElementType;    //!< Resulting element type.

   //! Return type for expression template evaluations.
   typedef const typename SelectType< returnExpr, ExprReturnType, ElementType >::Type  ReturnType;

   //! Data type for composite expression templates.
   typedef const ResultType  CompositeType;

   //! Composite type of the left-hand side dense vector expression.
   typedef typename SelectType< IsExpression<VT1>::value, const VT1, const VT1& >::Type  LeftOperand;

   //! Composite type of the right-hand side dense vector expression.
   typedef typename SelectType< IsExpression<VT2>::value, const VT2, const VT2& >::Type  RightOperand;

   //! Composite type of the left-hand side dense vector expression.
   typedef typename SelectType< IsComputation<VT1>::value, const StaticVector<ET1,3UL,false>, CT1 >::Type  LT;

   //! Composite type of the right-hand side dense vector expression.
   typedef typename SelectType< IsComputation<VT2>::value, const StaticVector<ET2,3UL,false>, CT2 >::Type  RT;
   //**********************************************************************************************

   //**Compilation flags***************************************************************************
   //! Compilation switch for the expression template evaluation strategy.
   enum { vectorizable = 0 };

   //! Compilation flag for the detection of aliasing effects.
   enum { canAlias = ( !IsComputation<VT1>::value || !IsComputation<VT2>::value ) };
   //**********************************************************************************************

   //**Constructor*********************************************************************************
   /*!\brief Constructor for the DVecDVecCrossExpr class.
   //
   // \param lhs The left-hand side operand of the cross product expression.
   // \param rhs The right-hand side operand of the cross product expression.
   */
   explicit inline DVecDVecCrossExpr( const VT1& lhs, const VT2& rhs )
      : lhs_( lhs )  // Left-hand side dense vector of the cross product expression
      , rhs_( rhs )  // Right-hand side dense vector of the cross product expression
   {
      BLAZE_INTERNAL_ASSERT( lhs.size() == 3UL, "Invalid vector size" );
      BLAZE_INTERNAL_ASSERT( rhs.size() == 3UL, "Invalid vector size" );
   }
   //**********************************************************************************************

   //**Subscript operator**************************************************************************
   /*!\brief Subscript operator for the direct access to the vector elements.
   //
   // \param index Access index. The index has to be in the range \f$[0..2]\f$.
   // \return The resulting value.
   */
   inline ReturnType operator[]( size_t index ) const {
      BLAZE_INTERNAL_ASSERT( index < 2UL, "Invalid vector access index" );

      if( index == 0UL )
         return lhs_[1UL] * rhs_[2UL] - lhs_[2UL] * rhs_[1UL];
      else if( index == 1UL )
         return lhs_[2UL] * rhs_[0UL] - lhs_[0UL] * rhs_[2UL];
      else
         return lhs_[0UL] * rhs_[1UL] - lhs_[1UL] * rhs_[0UL];
   }
   //**********************************************************************************************

   //**Size function*******************************************************************************
   /*!\brief Returns the current size/dimension of the vector.
   //
   // \return The size of the vector.
   */
   inline size_t size() const {
      return 3UL;
   }
   //**********************************************************************************************

   //**Left operand access*************************************************************************
   /*!\brief Returns the left-hand side dense vector operand.
   //
   // \return The left-hand side dense vector operand.
   */
   inline LeftOperand leftOperand() const {
      return lhs_;
   }
   //**********************************************************************************************

   //**Right operand access************************************************************************
   /*!\brief Returns the right-hand side dense vector operand.
   //
   // \return The right-hand side dense vector operand.
   */
   inline RightOperand rightOperand() const {
      return rhs_;
   }
   //**********************************************************************************************

   //**********************************************************************************************
   /*!\brief Returns whether the expression is aliased with the given address \a alias.
   //
   // \param alias The alias to be checked.
   // \return \a true in case an alias effect is detected, \a false otherwise.
   */
   template< typename T >
   inline bool isAliased( const T* alias ) const {
      return ( !IsComputation<VT1>::value && lhs_.isAliased( alias ) ) ||
             ( !IsComputation<VT2>::value && rhs_.isAliased( alias ) );
   }
   //**********************************************************************************************

 private:
   //**Member variables****************************************************************************
   LeftOperand  lhs_;  //!< Left-hand side dense vector of the cross product expression.
   RightOperand rhs_;  //!< Right-hand side dense vector of the cross product expression.
   //**********************************************************************************************

   //**Assignment to dense vectors*****************************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Assignment of a dense vector-dense vector cross product to a dense vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side dense vector.
   // \param rhs The right-hand side cross product expression to be assigned.
   // \return void
   //
   // This function implements the performance optimized assignment of a dense vector-dense
   // vector cross product expression to a dense vector. Due to the explicit application of
   // the SFINAE principle, this operator can only be selected by the compiler in case either
   // of the two operands requires an intermediate evaluation.
   */
   template< typename VT >  // Type of the target dense vector
   friend inline void assign( DenseVector<VT,false>& lhs, const DVecDVecCrossExpr& rhs )
   {
      BLAZE_INTERNAL_ASSERT( (~lhs).size() == 3UL, "Invalid vector size" );
      BLAZE_INTERNAL_ASSERT( (~rhs).size() == 3UL, "Invalid vector size" );

      LT x( rhs.lhs_ );  // Evaluation of the left-hand side dense vector operand
      RT y( rhs.rhs_ );  // Evaluation of the right-hand side dense vector operand

      (~lhs)[0] = x[1UL]*y[2UL] - x[2UL]*y[1UL];
      (~lhs)[1] = x[2UL]*y[0UL] - x[0UL]*y[2UL];
      (~lhs)[2] = x[0UL]*y[1UL] - x[1UL]*y[0UL];
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Assignment to sparse vectors****************************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Assignment of a dense vector-dense vector cross product to a sparse vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side sparse vector.
   // \param rhs The right-hand side cross product expression to be assigned.
   // \return void
   //
   // This function implements the performance optimized assignment of a dense vector-dense
   // vector cross product expression to a sparse vector. Due to the explicit application of
   // the SFINAE principle, this operator can only be selected by the compiler in case either
   // of the two operands requires an intermediate evaluation.
   */
   template< typename VT >  // Type of the target sparse vector
   friend inline void assign( SparseVector<VT,false>& lhs, const DVecDVecCrossExpr& rhs )
   {
      BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE( ResultType );
      BLAZE_CONSTRAINT_MUST_BE_NONTRANSPOSE_VECTOR_TYPE( ResultType );
      BLAZE_CONSTRAINT_MUST_BE_REFERENCE_TYPE( typename ResultType::CompositeType );

      BLAZE_INTERNAL_ASSERT( (~lhs).size() == 3UL, "Invalid vector size" );
      BLAZE_INTERNAL_ASSERT( (~rhs).size() == 3UL, "Invalid vector size" );

      const ResultType tmp( rhs );
      assign( ~lhs, tmp );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Addition assignment to dense vectors********************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Addition assignment of a dense vector-dense vector cross product to a dense vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side dense vector.
   // \param rhs The right-hand side cross product expression to be added.
   // \return void
   //
   // This function implements the performance optimized addition assignment of a dense vector-
   // dense vector cross product expression to a dense vector. Due to the explicit application
   // of the SFINAE principle, this operator can only be selected by the compiler in case either
   // of the operands requires an intermediate evaluation.
   */
   template< typename VT >  // Type of the target dense vector
   friend inline void addAssign( DenseVector<VT,false>& lhs, const DVecDVecCrossExpr& rhs )
   {
      BLAZE_INTERNAL_ASSERT( (~lhs).size() == 3UL, "Invalid vector size" );
      BLAZE_INTERNAL_ASSERT( (~rhs).size() == 3UL, "Invalid vector size" );

      LT x( rhs.lhs_ );  // Evaluation of the left-hand side dense vector operand
      RT y( rhs.rhs_ );  // Evaluation of the right-hand side dense vector operand

      (~lhs)[0] += x[1UL]*y[2UL] - x[2UL]*y[1UL];
      (~lhs)[1] += x[2UL]*y[0UL] - x[0UL]*y[2UL];
      (~lhs)[2] += x[0UL]*y[1UL] - x[1UL]*y[0UL];
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Addition assignment to sparse vectors*******************************************************
   // No special implementation for the addition assignment to sparse vectors.
   //**********************************************************************************************

   //**Subtraction assignment to dense vectors*****************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Subtraction assignment of a dense vector-dense vector cross product to a dense vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side dense vector.
   // \param rhs The right-hand side cross product expression to be subtracted.
   // \return void
   //
   // This function implements the performance optimized subtraction assignment of a dense vector-
   // dense vector cross product expression to a dense vector. Due to the explicit application of
   // the SFINAE principle, this operator can only be selected by the compiler in case either of
   // the operands requires an intermediate evaluation.
   */
   template< typename VT >  // Type of the target dense vector
   friend inline void subAssign( DenseVector<VT,false>& lhs, const DVecDVecCrossExpr& rhs )
   {
      BLAZE_INTERNAL_ASSERT( (~lhs).size() == 3UL, "Invalid vector size" );
      BLAZE_INTERNAL_ASSERT( (~rhs).size() == 3UL, "Invalid vector size" );

      LT x( rhs.lhs_ );  // Evaluation of the left-hand side dense vector operand
      RT y( rhs.rhs_ );  // Evaluation of the right-hand side dense vector operand

      (~lhs)[0] -= x[1UL]*y[2UL] - x[2UL]*y[1UL];
      (~lhs)[1] -= x[2UL]*y[0UL] - x[0UL]*y[2UL];
      (~lhs)[2] -= x[0UL]*y[1UL] - x[1UL]*y[0UL];
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Subtraction assignment to sparse vectors****************************************************
   // No special implementation for the subtraction assignment to sparse vectors.
   //**********************************************************************************************

   //**Multiplication assignment to dense vectors**************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Multiplication assignment of a dense vector-dense vector cross product to a dense vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side dense vector.
   // \param rhs The right-hand side cross product expression to be multiplied.
   // \return void
   //
   // This function implements the performance optimized multiplication assignment of a dense
   // vector-dense vector cross product expression to a dense vector. Due to the explicit
   // application of the SFINAE principle, this operator can only be selected by the compiler
   // in case either of the operands requires an intermediate evaluation.
   */
   template< typename VT >  // Type of the target dense vector
   friend inline void multAssign( DenseVector<VT,false>& lhs, const DVecDVecCrossExpr& rhs )
   {
      BLAZE_INTERNAL_ASSERT( (~lhs).size() == 3UL, "Invalid vector size" );
      BLAZE_INTERNAL_ASSERT( (~rhs).size() == 3UL, "Invalid vector size" );

      LT x( rhs.lhs_ );  // Evaluation of the left-hand side dense vector operand
      RT y( rhs.rhs_ );  // Evaluation of the right-hand side dense vector operand

      (~lhs)[0] *= x[1UL]*y[2UL] - x[2UL]*y[1UL];
      (~lhs)[1] *= x[2UL]*y[0UL] - x[0UL]*y[2UL];
      (~lhs)[2] *= x[0UL]*y[1UL] - x[1UL]*y[0UL];
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Multiplication assignment to sparse vectors*************************************************
   // No special implementation for the multiplication assignment to sparse vectors.
   //**********************************************************************************************

   //**Compile time checks*************************************************************************
   /*! \cond BLAZE_INTERNAL */
   BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE( VT1 );
   BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE( VT2 );
   BLAZE_CONSTRAINT_MUST_BE_NONTRANSPOSE_VECTOR_TYPE( VT1 );
   BLAZE_CONSTRAINT_MUST_BE_NONTRANSPOSE_VECTOR_TYPE( VT2 );
   /*! \endcond */
   //**********************************************************************************************
};
//*************************************************************************************************




//=================================================================================================
//
//  GLOBAL BINARY ARITHMETIC OPERATORS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Operator for the cross product of two dense vectors (\f$ \vec{a}=\vec{b} \times \vec{c} \f$).
// \ingroup dense_vector
//
// \param lhs The left-hand side dense vector for the cross product.
// \param rhs The right-hand side dense vector for the cross product.
// \return The cross product of the two vectors.
// \exception std::invalid_argument Invalid vector size for cross product.
//
// This operator represents the cross product of two dense vectors:

   \code
   blaze::DynamicVector<double> a( 3UL ), b( 3UL );
   blaze::StaticVector<double,3UL> c;
   // ... Resizing and initialization
   c = a % b;
   \endcode

// The operator returns an expression representing a dense vector of the higher-order element
// type of the two involved vector element types \a T1::ElementType and \a T2::ElementType.
// Both vector types \a T1 and \a T2 as well as the two element types \a T1::ElementType and
// \a T2::ElementType have to be supported by the CrossTrait class template.\n
// In case the current sizes of the two given vectors don't match, a \a std::invalid_argument
// is thrown.
*/
template< typename T1    // Type of the left-hand side dense vector
        , typename T2 >  // Type of the right-hand side dense vector
inline const DVecDVecCrossExpr<T1,T2>
   operator%( const DenseVector<T1,false>& lhs, const DenseVector<T2,false>& rhs )
{
   if( (~lhs).size() != 3UL || (~rhs).size() != 3UL )
      throw std::invalid_argument( "Invalid vector size for cross product" );

   return DVecDVecCrossExpr<T1,T2>( ~lhs, ~rhs );
}
//*************************************************************************************************

} // namespace blaze

#endif
