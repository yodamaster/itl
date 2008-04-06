/*----------------------------------------------------------------------------+
Copyright (c) 2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
+----------------------------------------------------------------------------*/

//in addition:
//subst CTX::<T>				itl::<T>
#define SetT					itl::set
#define MapT					itl::map
#define VectorT					itl::vector
#define PtrVectorT				itl::ptr_vector
#define StringSetT				itl::string_set
#define StringMapT				itl::string_map
#define StringPatternSetT		itl::stringpattern_set
//#define ListT					itl::list
#define IntervalT				itl::interval
#define BOUNDTYPES				bound_types
#define ItvSetT					itl::interval_set
#define DiscItvSetT				itl::split_interval_set
#define SplitItvMapT			itl::split_interval_map
#define StringListT				itl::string_list
#define StringSheetT			itl::string_sheet
#define DiffDescT				itl::diff_desc
#define VarPermutationT			itl::var_permutation
#define OrderedTypeIT			itl::ordered_type			
#define OrderedTypeAT			itl::ordered_type_base		
#define TypedEpisodeAT			itl::typed_episode			
#define EpisodeIT				itl::episode_interface		
#define EpisodeProductT			itl::episode_product
#define ProductHistoryT			itl::product_history
#define EpisodeSetT				itl::episode_set
#define FastVarTupelT			itl::var_tuple				
#define GroupOrderIT			itl::group_order			
#define GroupingT				itl::grouping			
#define VarTupelOrderT			itl::var_tuple_order
#define TupelOrderTD            tuple_order_type;
#define VarTupelInscriptorAT	itl::var_tuple_inscriptor	
#define TupelComputerIT			itl::tuple_computer_interface 
#define TupelComputerAT			itl::tuple_computer_base	
#define AmountTupelComputerT	itl::amount_tuple_computer  
#define DateTupelComputerT		itl::date_tuple_computer	
#define ItvTupelComputerT		itl::interval_tuple_computer
#define AmountComputer			itl::amount_computer		
#define DateComputer			itl::date_computer			
#define ItvComputer				itl::interval_computer		
#define StatComputerT			itl::stat_computer			
#define DVarVectorTD			itl::dvar_vector_type
#define IndexLimitsT			itl::index_limits			
#define DependVarSignatT		itl::depend_var_signat
#define DomainTD				domain_type
#define CodomainTD				codomain_type
#define IntervalTD				interval_type
#define isClosed				is_closed
#define isOpen					is_open
#define isLOpen					is_leftopen
#define isROpen					is_rightopen
#define isRightOpen				rightbound_open
#define isLeftOpen				leftbound_open
#define isRightClose			rightbound_closed
#define isLeftClose				leftbound_closed
#define closedInterval			itl::closed_interval
#define rightOpenInterval		itl::rightopen_interval
#define leftOpenInterval		itl::leftopen_interval
#define openInterval			itl::open_interval
#define isEqual					equal
#define isSubsetOf				contained_in
#define isDisjointTo			disjoint_to
#define scaleUp					scale_up
#define scaleDown				scale_down
#define leftSurplus				left_surplus
#define rightSurplus			right_surplus
#define isEqual					equal
#define handleNeighbours		handle_neighbours
#define fst_itv					first_interval
#define lst_itv					last_interval
#define nOfIntervals			interval_count
#define uniformBounds			uniform_bounds
#define consequentPermutation	consequent_permutation
#define setUpb					set_upb
#define setLwb					set_lwb
#define fst						first
#define lst						last
#define asClosedInterval		as_closed_interval
#define asRightOpenInterval		as_rightopen_interval
#define closeLeftBound			close_left_bound
#define openRightBound			open_right_bound
#define excl_less				exclusive_less

