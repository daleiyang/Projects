
B
input_xPlaceholder*
dtype0*
shape:˙˙˙˙˙˙˙˙˙Ř
A
input_yPlaceholder*
dtype0*
shape:˙˙˙˙˙˙˙˙˙
4
	keep_probPlaceholder*
dtype0*
shape:
}
*embedding/Initializer/random_uniform/shapeConst*
valueB"  @   *
_class
loc:@embedding*
dtype0
s
(embedding/Initializer/random_uniform/minConst*
valueB
 *yý˝*
_class
loc:@embedding*
dtype0
s
(embedding/Initializer/random_uniform/maxConst*
valueB
 *yý=*
_class
loc:@embedding*
dtype0
ź
2embedding/Initializer/random_uniform/RandomUniformRandomUniform*embedding/Initializer/random_uniform/shape*

seed *
T0*
_class
loc:@embedding*
dtype0*
seed2 
Ş
(embedding/Initializer/random_uniform/subSub(embedding/Initializer/random_uniform/max(embedding/Initializer/random_uniform/min*
T0*
_class
loc:@embedding
´
(embedding/Initializer/random_uniform/mulMul2embedding/Initializer/random_uniform/RandomUniform(embedding/Initializer/random_uniform/sub*
T0*
_class
loc:@embedding
Ś
$embedding/Initializer/random_uniformAdd(embedding/Initializer/random_uniform/mul(embedding/Initializer/random_uniform/min*
T0*
_class
loc:@embedding

	embedding
VariableV2"/device:CPU:0*
_class
loc:@embedding*
dtype0*
	container *
shape:	'@*
shared_name 
Ş
embedding/AssignAssign	embedding$embedding/Initializer/random_uniform"/device:CPU:0*
T0*
_class
loc:@embedding*
validate_shape(*
use_locking(
[
embedding/readIdentity	embedding"/device:CPU:0*
T0*
_class
loc:@embedding
l
embedding_lookup/axisConst"/device:CPU:0*
value	B : *
_class
loc:@embedding*
dtype0
Ľ
embedding_lookupGatherV2embedding/readinput_xembedding_lookup/axis"/device:CPU:0*
Tparams0*
_class
loc:@embedding*
Taxis0*
Tindices0
O
embedding_lookup/IdentityIdentityembedding_lookup"/device:CPU:0*
T0

,conv/kernel/Initializer/random_uniform/shapeConst*!
valueB"   @      *
_class
loc:@conv/kernel*
dtype0
w
*conv/kernel/Initializer/random_uniform/minConst*
valueB
 *çÓz˝*
_class
loc:@conv/kernel*
dtype0
w
*conv/kernel/Initializer/random_uniform/maxConst*
valueB
 *çÓz=*
_class
loc:@conv/kernel*
dtype0
Â
4conv/kernel/Initializer/random_uniform/RandomUniformRandomUniform,conv/kernel/Initializer/random_uniform/shape*

seed *
T0*
_class
loc:@conv/kernel*
dtype0*
seed2 
˛
*conv/kernel/Initializer/random_uniform/subSub*conv/kernel/Initializer/random_uniform/max*conv/kernel/Initializer/random_uniform/min*
T0*
_class
loc:@conv/kernel
ź
*conv/kernel/Initializer/random_uniform/mulMul4conv/kernel/Initializer/random_uniform/RandomUniform*conv/kernel/Initializer/random_uniform/sub*
T0*
_class
loc:@conv/kernel
Ž
&conv/kernel/Initializer/random_uniformAdd*conv/kernel/Initializer/random_uniform/mul*conv/kernel/Initializer/random_uniform/min*
T0*
_class
loc:@conv/kernel

conv/kernel
VariableV2*
_class
loc:@conv/kernel*
dtype0*
	container *
shape:@*
shared_name 
Ł
conv/kernel/AssignAssignconv/kernel&conv/kernel/Initializer/random_uniform*
use_locking(*
T0*
_class
loc:@conv/kernel*
validate_shape(
R
conv/kernel/readIdentityconv/kernel*
T0*
_class
loc:@conv/kernel
k
conv/bias/Initializer/zerosConst*
valueB*    *
_class
loc:@conv/bias*
dtype0
x
	conv/bias
VariableV2*
shape:*
shared_name *
_class
loc:@conv/bias*
dtype0*
	container 

conv/bias/AssignAssign	conv/biasconv/bias/Initializer/zeros*
T0*
_class
loc:@conv/bias*
validate_shape(*
use_locking(
L
conv/bias/readIdentity	conv/bias*
T0*
_class
loc:@conv/bias
D
cnn/conv/dilation_rateConst*
valueB:*
dtype0
H
cnn/conv/conv1d/ExpandDims/dimConst*
value	B :*
dtype0
x
cnn/conv/conv1d/ExpandDims
ExpandDimsembedding_lookup/Identitycnn/conv/conv1d/ExpandDims/dim*
T0*

Tdim0
J
 cnn/conv/conv1d/ExpandDims_1/dimConst*
value	B : *
dtype0
s
cnn/conv/conv1d/ExpandDims_1
ExpandDimsconv/kernel/read cnn/conv/conv1d/ExpandDims_1/dim*

Tdim0*
T0
Ň
cnn/conv/conv1d/Conv2DConv2Dcnn/conv/conv1d/ExpandDimscnn/conv/conv1d/ExpandDims_1*
	dilations
*
T0*
data_formatNHWC*
strides
*
use_cudnn_on_gpu(*
paddingVALID
Z
cnn/conv/conv1d/SqueezeSqueezecnn/conv/conv1d/Conv2D*
T0*
squeeze_dims

d
cnn/conv/BiasAddBiasAddcnn/conv/conv1d/Squeezeconv/bias/read*
T0*
data_formatNHWC
G
cnn/gmp/reduction_indicesConst*
valueB:*
dtype0
a
cnn/gmpMaxcnn/conv/BiasAddcnn/gmp/reduction_indices*

Tidx0*
	keep_dims( *
T0

+fc1/kernel/Initializer/random_uniform/shapeConst*
valueB"      *
_class
loc:@fc1/kernel*
dtype0
u
)fc1/kernel/Initializer/random_uniform/minConst*
valueB
 *   ž*
_class
loc:@fc1/kernel*
dtype0
u
)fc1/kernel/Initializer/random_uniform/maxConst*
valueB
 *   >*
_class
loc:@fc1/kernel*
dtype0
ż
3fc1/kernel/Initializer/random_uniform/RandomUniformRandomUniform+fc1/kernel/Initializer/random_uniform/shape*

seed *
T0*
_class
loc:@fc1/kernel*
dtype0*
seed2 
Ž
)fc1/kernel/Initializer/random_uniform/subSub)fc1/kernel/Initializer/random_uniform/max)fc1/kernel/Initializer/random_uniform/min*
T0*
_class
loc:@fc1/kernel
¸
)fc1/kernel/Initializer/random_uniform/mulMul3fc1/kernel/Initializer/random_uniform/RandomUniform)fc1/kernel/Initializer/random_uniform/sub*
T0*
_class
loc:@fc1/kernel
Ş
%fc1/kernel/Initializer/random_uniformAdd)fc1/kernel/Initializer/random_uniform/mul)fc1/kernel/Initializer/random_uniform/min*
T0*
_class
loc:@fc1/kernel


fc1/kernel
VariableV2*
_class
loc:@fc1/kernel*
dtype0*
	container *
shape:
*
shared_name 

fc1/kernel/AssignAssign
fc1/kernel%fc1/kernel/Initializer/random_uniform*
use_locking(*
T0*
_class
loc:@fc1/kernel*
validate_shape(
O
fc1/kernel/readIdentity
fc1/kernel*
T0*
_class
loc:@fc1/kernel
i
fc1/bias/Initializer/zerosConst*
valueB*    *
_class
loc:@fc1/bias*
dtype0
v
fc1/bias
VariableV2*
_class
loc:@fc1/bias*
dtype0*
	container *
shape:*
shared_name 

fc1/bias/AssignAssignfc1/biasfc1/bias/Initializer/zeros*
use_locking(*
T0*
_class
loc:@fc1/bias*
validate_shape(
I
fc1/bias/readIdentityfc1/bias*
T0*
_class
loc:@fc1/bias
c
score/fc1/MatMulMatMulcnn/gmpfc1/kernel/read*
transpose_b( *
T0*
transpose_a( 
]
score/fc1/BiasAddBiasAddscore/fc1/MatMulfc1/bias/read*
T0*
data_formatNHWC
@
score/Dropout/sub/xConst*
valueB
 *  ?*
dtype0
A
score/Dropout/subSubscore/Dropout/sub/x	keep_prob*
T0
B
score/Dropout/sub_1/xConst*
valueB
 *  ?*
dtype0
M
score/Dropout/sub_1Subscore/Dropout/sub_1/xscore/Dropout/sub*
T0
R
score/Dropout/dropout_1/ShapeShapescore/fc1/BiasAdd*
T0*
out_type0
W
*score/Dropout/dropout_1/random_uniform/minConst*
valueB
 *    *
dtype0
W
*score/Dropout/dropout_1/random_uniform/maxConst*
valueB
 *  ?*
dtype0

4score/Dropout/dropout_1/random_uniform/RandomUniformRandomUniformscore/Dropout/dropout_1/Shape*
T0*
dtype0*
seed2 *

seed 

*score/Dropout/dropout_1/random_uniform/subSub*score/Dropout/dropout_1/random_uniform/max*score/Dropout/dropout_1/random_uniform/min*
T0

*score/Dropout/dropout_1/random_uniform/mulMul4score/Dropout/dropout_1/random_uniform/RandomUniform*score/Dropout/dropout_1/random_uniform/sub*
T0

&score/Dropout/dropout_1/random_uniformAdd*score/Dropout/dropout_1/random_uniform/mul*score/Dropout/dropout_1/random_uniform/min*
T0
h
score/Dropout/dropout_1/addAddscore/Dropout/sub_1&score/Dropout/dropout_1/random_uniform*
T0
L
score/Dropout/dropout_1/FloorFloorscore/Dropout/dropout_1/add*
T0
W
score/Dropout/dropout_1/divRealDivscore/fc1/BiasAddscore/Dropout/sub_1*
T0
g
score/Dropout/dropout_1/mulMulscore/Dropout/dropout_1/divscore/Dropout/dropout_1/Floor*
T0
8

score/ReluReluscore/Dropout/dropout_1/mul*
T0

+fc2/kernel/Initializer/random_uniform/shapeConst*
valueB"      *
_class
loc:@fc2/kernel*
dtype0
u
)fc2/kernel/Initializer/random_uniform/minConst*
valueB
 *JQZž*
_class
loc:@fc2/kernel*
dtype0
u
)fc2/kernel/Initializer/random_uniform/maxConst*
valueB
 *JQZ>*
_class
loc:@fc2/kernel*
dtype0
ż
3fc2/kernel/Initializer/random_uniform/RandomUniformRandomUniform+fc2/kernel/Initializer/random_uniform/shape*

seed *
T0*
_class
loc:@fc2/kernel*
dtype0*
seed2 
Ž
)fc2/kernel/Initializer/random_uniform/subSub)fc2/kernel/Initializer/random_uniform/max)fc2/kernel/Initializer/random_uniform/min*
T0*
_class
loc:@fc2/kernel
¸
)fc2/kernel/Initializer/random_uniform/mulMul3fc2/kernel/Initializer/random_uniform/RandomUniform)fc2/kernel/Initializer/random_uniform/sub*
T0*
_class
loc:@fc2/kernel
Ş
%fc2/kernel/Initializer/random_uniformAdd)fc2/kernel/Initializer/random_uniform/mul)fc2/kernel/Initializer/random_uniform/min*
T0*
_class
loc:@fc2/kernel
~

fc2/kernel
VariableV2*
_class
loc:@fc2/kernel*
dtype0*
	container *
shape:	*
shared_name 

fc2/kernel/AssignAssign
fc2/kernel%fc2/kernel/Initializer/random_uniform*
T0*
_class
loc:@fc2/kernel*
validate_shape(*
use_locking(
O
fc2/kernel/readIdentity
fc2/kernel*
T0*
_class
loc:@fc2/kernel
h
fc2/bias/Initializer/zerosConst*
valueB*    *
_class
loc:@fc2/bias*
dtype0
u
fc2/bias
VariableV2*
_class
loc:@fc2/bias*
dtype0*
	container *
shape:*
shared_name 

fc2/bias/AssignAssignfc2/biasfc2/bias/Initializer/zeros*
T0*
_class
loc:@fc2/bias*
validate_shape(*
use_locking(
I
fc2/bias/readIdentityfc2/bias*
T0*
_class
loc:@fc2/bias
f
score/fc2/MatMulMatMul
score/Relufc2/kernel/read*
transpose_b( *
T0*
transpose_a( 
]
score/fc2/BiasAddBiasAddscore/fc2/MatMulfc2/bias/read*
T0*
data_formatNHWC
4
score/SoftmaxSoftmaxscore/fc2/BiasAdd*
T0
D
score/y_pred_cls/dimensionConst*
value	B :*
dtype0
m
score/y_pred_clsArgMaxscore/Softmaxscore/y_pred_cls/dimension*
T0*
output_type0	*

Tidx0
3
score/y_probSoftmaxscore/fc2/BiasAdd*
T0
d
Boptimize/softmax_cross_entropy_with_logits_sg/labels_stop_gradientStopGradientinput_y*
T0
\
2optimize/softmax_cross_entropy_with_logits_sg/RankConst*
value	B :*
dtype0
h
3optimize/softmax_cross_entropy_with_logits_sg/ShapeShapescore/fc2/BiasAdd*
T0*
out_type0
^
4optimize/softmax_cross_entropy_with_logits_sg/Rank_1Const*
value	B :*
dtype0
j
5optimize/softmax_cross_entropy_with_logits_sg/Shape_1Shapescore/fc2/BiasAdd*
T0*
out_type0
]
3optimize/softmax_cross_entropy_with_logits_sg/Sub/yConst*
value	B :*
dtype0
Ź
1optimize/softmax_cross_entropy_with_logits_sg/SubSub4optimize/softmax_cross_entropy_with_logits_sg/Rank_13optimize/softmax_cross_entropy_with_logits_sg/Sub/y*
T0

9optimize/softmax_cross_entropy_with_logits_sg/Slice/beginPack1optimize/softmax_cross_entropy_with_logits_sg/Sub*
T0*

axis *
N
f
8optimize/softmax_cross_entropy_with_logits_sg/Slice/sizeConst*
valueB:*
dtype0
ţ
3optimize/softmax_cross_entropy_with_logits_sg/SliceSlice5optimize/softmax_cross_entropy_with_logits_sg/Shape_19optimize/softmax_cross_entropy_with_logits_sg/Slice/begin8optimize/softmax_cross_entropy_with_logits_sg/Slice/size*
T0*
Index0
t
=optimize/softmax_cross_entropy_with_logits_sg/concat/values_0Const*
valueB:
˙˙˙˙˙˙˙˙˙*
dtype0
c
9optimize/softmax_cross_entropy_with_logits_sg/concat/axisConst*
value	B : *
dtype0

4optimize/softmax_cross_entropy_with_logits_sg/concatConcatV2=optimize/softmax_cross_entropy_with_logits_sg/concat/values_03optimize/softmax_cross_entropy_with_logits_sg/Slice9optimize/softmax_cross_entropy_with_logits_sg/concat/axis*
T0*
N*

Tidx0
 
5optimize/softmax_cross_entropy_with_logits_sg/ReshapeReshapescore/fc2/BiasAdd4optimize/softmax_cross_entropy_with_logits_sg/concat*
T0*
Tshape0
^
4optimize/softmax_cross_entropy_with_logits_sg/Rank_2Const*
value	B :*
dtype0

5optimize/softmax_cross_entropy_with_logits_sg/Shape_2ShapeBoptimize/softmax_cross_entropy_with_logits_sg/labels_stop_gradient*
T0*
out_type0
_
5optimize/softmax_cross_entropy_with_logits_sg/Sub_1/yConst*
value	B :*
dtype0
°
3optimize/softmax_cross_entropy_with_logits_sg/Sub_1Sub4optimize/softmax_cross_entropy_with_logits_sg/Rank_25optimize/softmax_cross_entropy_with_logits_sg/Sub_1/y*
T0

;optimize/softmax_cross_entropy_with_logits_sg/Slice_1/beginPack3optimize/softmax_cross_entropy_with_logits_sg/Sub_1*
T0*

axis *
N
h
:optimize/softmax_cross_entropy_with_logits_sg/Slice_1/sizeConst*
valueB:*
dtype0

5optimize/softmax_cross_entropy_with_logits_sg/Slice_1Slice5optimize/softmax_cross_entropy_with_logits_sg/Shape_2;optimize/softmax_cross_entropy_with_logits_sg/Slice_1/begin:optimize/softmax_cross_entropy_with_logits_sg/Slice_1/size*
T0*
Index0
v
?optimize/softmax_cross_entropy_with_logits_sg/concat_1/values_0Const*
valueB:
˙˙˙˙˙˙˙˙˙*
dtype0
e
;optimize/softmax_cross_entropy_with_logits_sg/concat_1/axisConst*
value	B : *
dtype0

6optimize/softmax_cross_entropy_with_logits_sg/concat_1ConcatV2?optimize/softmax_cross_entropy_with_logits_sg/concat_1/values_05optimize/softmax_cross_entropy_with_logits_sg/Slice_1;optimize/softmax_cross_entropy_with_logits_sg/concat_1/axis*
T0*
N*

Tidx0
Ő
7optimize/softmax_cross_entropy_with_logits_sg/Reshape_1ReshapeBoptimize/softmax_cross_entropy_with_logits_sg/labels_stop_gradient6optimize/softmax_cross_entropy_with_logits_sg/concat_1*
T0*
Tshape0
Ç
-optimize/softmax_cross_entropy_with_logits_sgSoftmaxCrossEntropyWithLogits5optimize/softmax_cross_entropy_with_logits_sg/Reshape7optimize/softmax_cross_entropy_with_logits_sg/Reshape_1*
T0
_
5optimize/softmax_cross_entropy_with_logits_sg/Sub_2/yConst*
value	B :*
dtype0
Ž
3optimize/softmax_cross_entropy_with_logits_sg/Sub_2Sub2optimize/softmax_cross_entropy_with_logits_sg/Rank5optimize/softmax_cross_entropy_with_logits_sg/Sub_2/y*
T0
i
;optimize/softmax_cross_entropy_with_logits_sg/Slice_2/beginConst*
valueB: *
dtype0

:optimize/softmax_cross_entropy_with_logits_sg/Slice_2/sizePack3optimize/softmax_cross_entropy_with_logits_sg/Sub_2*
T0*

axis *
N

5optimize/softmax_cross_entropy_with_logits_sg/Slice_2Slice3optimize/softmax_cross_entropy_with_logits_sg/Shape;optimize/softmax_cross_entropy_with_logits_sg/Slice_2/begin:optimize/softmax_cross_entropy_with_logits_sg/Slice_2/size*
T0*
Index0
ż
7optimize/softmax_cross_entropy_with_logits_sg/Reshape_2Reshape-optimize/softmax_cross_entropy_with_logits_sg5optimize/softmax_cross_entropy_with_logits_sg/Slice_2*
T0*
Tshape0
<
optimize/ConstConst*
valueB: *
dtype0

optimize/MeanMean7optimize/softmax_cross_entropy_with_logits_sg/Reshape_2optimize/Const*

Tidx0*
	keep_dims( *
T0
A
optimize/gradients/ShapeConst*
valueB *
dtype0
I
optimize/gradients/grad_ys_0Const*
valueB
 *  ?*
dtype0
r
optimize/gradients/FillFilloptimize/gradients/Shapeoptimize/gradients/grad_ys_0*
T0*

index_type0
a
3optimize/gradients/optimize/Mean_grad/Reshape/shapeConst*
valueB:*
dtype0

-optimize/gradients/optimize/Mean_grad/ReshapeReshapeoptimize/gradients/Fill3optimize/gradients/optimize/Mean_grad/Reshape/shape*
T0*
Tshape0

+optimize/gradients/optimize/Mean_grad/ShapeShape7optimize/softmax_cross_entropy_with_logits_sg/Reshape_2*
T0*
out_type0
Š
*optimize/gradients/optimize/Mean_grad/TileTile-optimize/gradients/optimize/Mean_grad/Reshape+optimize/gradients/optimize/Mean_grad/Shape*
T0*

Tmultiples0

-optimize/gradients/optimize/Mean_grad/Shape_1Shape7optimize/softmax_cross_entropy_with_logits_sg/Reshape_2*
T0*
out_type0
V
-optimize/gradients/optimize/Mean_grad/Shape_2Const*
valueB *
dtype0
Y
+optimize/gradients/optimize/Mean_grad/ConstConst*
valueB: *
dtype0
´
*optimize/gradients/optimize/Mean_grad/ProdProd-optimize/gradients/optimize/Mean_grad/Shape_1+optimize/gradients/optimize/Mean_grad/Const*

Tidx0*
	keep_dims( *
T0
[
-optimize/gradients/optimize/Mean_grad/Const_1Const*
valueB: *
dtype0
¸
,optimize/gradients/optimize/Mean_grad/Prod_1Prod-optimize/gradients/optimize/Mean_grad/Shape_2-optimize/gradients/optimize/Mean_grad/Const_1*

Tidx0*
	keep_dims( *
T0
Y
/optimize/gradients/optimize/Mean_grad/Maximum/yConst*
value	B :*
dtype0
 
-optimize/gradients/optimize/Mean_grad/MaximumMaximum,optimize/gradients/optimize/Mean_grad/Prod_1/optimize/gradients/optimize/Mean_grad/Maximum/y*
T0

.optimize/gradients/optimize/Mean_grad/floordivFloorDiv*optimize/gradients/optimize/Mean_grad/Prod-optimize/gradients/optimize/Mean_grad/Maximum*
T0

*optimize/gradients/optimize/Mean_grad/CastCast.optimize/gradients/optimize/Mean_grad/floordiv*

SrcT0*
Truncate( *

DstT0

-optimize/gradients/optimize/Mean_grad/truedivRealDiv*optimize/gradients/optimize/Mean_grad/Tile*optimize/gradients/optimize/Mean_grad/Cast*
T0
Ś
Uoptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg/Reshape_2_grad/ShapeShape-optimize/softmax_cross_entropy_with_logits_sg*
T0*
out_type0
˙
Woptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg/Reshape_2_grad/ReshapeReshape-optimize/gradients/optimize/Mean_grad/truedivUoptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg/Reshape_2_grad/Shape*
T0*
Tshape0
d
optimize/gradients/zeros_like	ZerosLike/optimize/softmax_cross_entropy_with_logits_sg:1*
T0

Toptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/ExpandDims/dimConst*
valueB :
˙˙˙˙˙˙˙˙˙*
dtype0
˘
Poptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/ExpandDims
ExpandDimsWoptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg/Reshape_2_grad/ReshapeToptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/ExpandDims/dim*
T0*

Tdim0
Ü
Ioptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/mulMulPoptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/ExpandDims/optimize/softmax_cross_entropy_with_logits_sg:1*
T0

Poptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/LogSoftmax
LogSoftmax5optimize/softmax_cross_entropy_with_logits_sg/Reshape*
T0
Ť
Ioptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/NegNegPoptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/LogSoftmax*
T0

Voptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/ExpandDims_1/dimConst*
valueB :
˙˙˙˙˙˙˙˙˙*
dtype0
Ś
Roptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/ExpandDims_1
ExpandDimsWoptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg/Reshape_2_grad/ReshapeVoptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/ExpandDims_1/dim*
T0*

Tdim0
ú
Koptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/mul_1MulRoptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/ExpandDims_1Ioptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/Neg*
T0
ř
Voptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/tuple/group_depsNoOpJ^optimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/mulL^optimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/mul_1
ő
^optimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/tuple/control_dependencyIdentityIoptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/mulW^optimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/tuple/group_deps*
T0*\
_classR
PNloc:@optimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/mul
ű
`optimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/tuple/control_dependency_1IdentityKoptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/mul_1W^optimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/tuple/group_deps*
T0*^
_classT
RPloc:@optimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/mul_1

Soptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg/Reshape_grad/ShapeShapescore/fc2/BiasAdd*
T0*
out_type0
Ź
Uoptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg/Reshape_grad/ReshapeReshape^optimize/gradients/optimize/softmax_cross_entropy_with_logits_sg_grad/tuple/control_dependencySoptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg/Reshape_grad/Shape*
T0*
Tshape0
ť
5optimize/gradients/score/fc2/BiasAdd_grad/BiasAddGradBiasAddGradUoptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg/Reshape_grad/Reshape*
T0*
data_formatNHWC
Ň
:optimize/gradients/score/fc2/BiasAdd_grad/tuple/group_depsNoOpV^optimize/gradients/optimize/softmax_cross_entropy_with_logits_sg/Reshape_grad/Reshape6^optimize/gradients/score/fc2/BiasAdd_grad/BiasAddGrad
Ő
Boptimize/gradients/score/fc2/BiasAdd_grad/tuple/control_dependencyIdentityUoptimize/gradients/optimize/softmax_cross_entropy_with_logits_sg/Reshape_grad/Reshape;^optimize/gradients/score/fc2/BiasAdd_grad/tuple/group_deps*
T0*h
_class^
\Zloc:@optimize/gradients/optimize/softmax_cross_entropy_with_logits_sg/Reshape_grad/Reshape

Doptimize/gradients/score/fc2/BiasAdd_grad/tuple/control_dependency_1Identity5optimize/gradients/score/fc2/BiasAdd_grad/BiasAddGrad;^optimize/gradients/score/fc2/BiasAdd_grad/tuple/group_deps*
T0*H
_class>
<:loc:@optimize/gradients/score/fc2/BiasAdd_grad/BiasAddGrad
˝
/optimize/gradients/score/fc2/MatMul_grad/MatMulMatMulBoptimize/gradients/score/fc2/BiasAdd_grad/tuple/control_dependencyfc2/kernel/read*
T0*
transpose_a( *
transpose_b(
ş
1optimize/gradients/score/fc2/MatMul_grad/MatMul_1MatMul
score/ReluBoptimize/gradients/score/fc2/BiasAdd_grad/tuple/control_dependency*
T0*
transpose_a(*
transpose_b( 
§
9optimize/gradients/score/fc2/MatMul_grad/tuple/group_depsNoOp0^optimize/gradients/score/fc2/MatMul_grad/MatMul2^optimize/gradients/score/fc2/MatMul_grad/MatMul_1

Aoptimize/gradients/score/fc2/MatMul_grad/tuple/control_dependencyIdentity/optimize/gradients/score/fc2/MatMul_grad/MatMul:^optimize/gradients/score/fc2/MatMul_grad/tuple/group_deps*
T0*B
_class8
64loc:@optimize/gradients/score/fc2/MatMul_grad/MatMul

Coptimize/gradients/score/fc2/MatMul_grad/tuple/control_dependency_1Identity1optimize/gradients/score/fc2/MatMul_grad/MatMul_1:^optimize/gradients/score/fc2/MatMul_grad/tuple/group_deps*
T0*D
_class:
86loc:@optimize/gradients/score/fc2/MatMul_grad/MatMul_1

+optimize/gradients/score/Relu_grad/ReluGradReluGradAoptimize/gradients/score/fc2/MatMul_grad/tuple/control_dependency
score/Relu*
T0
x
9optimize/gradients/score/Dropout/dropout_1/mul_grad/ShapeShapescore/Dropout/dropout_1/div*
T0*
out_type0
|
;optimize/gradients/score/Dropout/dropout_1/mul_grad/Shape_1Shapescore/Dropout/dropout_1/Floor*
T0*
out_type0
ă
Ioptimize/gradients/score/Dropout/dropout_1/mul_grad/BroadcastGradientArgsBroadcastGradientArgs9optimize/gradients/score/Dropout/dropout_1/mul_grad/Shape;optimize/gradients/score/Dropout/dropout_1/mul_grad/Shape_1*
T0

7optimize/gradients/score/Dropout/dropout_1/mul_grad/MulMul+optimize/gradients/score/Relu_grad/ReluGradscore/Dropout/dropout_1/Floor*
T0
č
7optimize/gradients/score/Dropout/dropout_1/mul_grad/SumSum7optimize/gradients/score/Dropout/dropout_1/mul_grad/MulIoptimize/gradients/score/Dropout/dropout_1/mul_grad/BroadcastGradientArgs*

Tidx0*
	keep_dims( *
T0
Ń
;optimize/gradients/score/Dropout/dropout_1/mul_grad/ReshapeReshape7optimize/gradients/score/Dropout/dropout_1/mul_grad/Sum9optimize/gradients/score/Dropout/dropout_1/mul_grad/Shape*
T0*
Tshape0

9optimize/gradients/score/Dropout/dropout_1/mul_grad/Mul_1Mulscore/Dropout/dropout_1/div+optimize/gradients/score/Relu_grad/ReluGrad*
T0
î
9optimize/gradients/score/Dropout/dropout_1/mul_grad/Sum_1Sum9optimize/gradients/score/Dropout/dropout_1/mul_grad/Mul_1Koptimize/gradients/score/Dropout/dropout_1/mul_grad/BroadcastGradientArgs:1*
T0*

Tidx0*
	keep_dims( 
×
=optimize/gradients/score/Dropout/dropout_1/mul_grad/Reshape_1Reshape9optimize/gradients/score/Dropout/dropout_1/mul_grad/Sum_1;optimize/gradients/score/Dropout/dropout_1/mul_grad/Shape_1*
T0*
Tshape0
Ę
Doptimize/gradients/score/Dropout/dropout_1/mul_grad/tuple/group_depsNoOp<^optimize/gradients/score/Dropout/dropout_1/mul_grad/Reshape>^optimize/gradients/score/Dropout/dropout_1/mul_grad/Reshape_1
ľ
Loptimize/gradients/score/Dropout/dropout_1/mul_grad/tuple/control_dependencyIdentity;optimize/gradients/score/Dropout/dropout_1/mul_grad/ReshapeE^optimize/gradients/score/Dropout/dropout_1/mul_grad/tuple/group_deps*
T0*N
_classD
B@loc:@optimize/gradients/score/Dropout/dropout_1/mul_grad/Reshape
ť
Noptimize/gradients/score/Dropout/dropout_1/mul_grad/tuple/control_dependency_1Identity=optimize/gradients/score/Dropout/dropout_1/mul_grad/Reshape_1E^optimize/gradients/score/Dropout/dropout_1/mul_grad/tuple/group_deps*
T0*P
_classF
DBloc:@optimize/gradients/score/Dropout/dropout_1/mul_grad/Reshape_1
n
9optimize/gradients/score/Dropout/dropout_1/div_grad/ShapeShapescore/fc1/BiasAdd*
T0*
out_type0
r
;optimize/gradients/score/Dropout/dropout_1/div_grad/Shape_1Shapescore/Dropout/sub_1*
T0*
out_type0
ă
Ioptimize/gradients/score/Dropout/dropout_1/div_grad/BroadcastGradientArgsBroadcastGradientArgs9optimize/gradients/score/Dropout/dropout_1/div_grad/Shape;optimize/gradients/score/Dropout/dropout_1/div_grad/Shape_1*
T0
˛
;optimize/gradients/score/Dropout/dropout_1/div_grad/RealDivRealDivLoptimize/gradients/score/Dropout/dropout_1/mul_grad/tuple/control_dependencyscore/Dropout/sub_1*
T0
ě
7optimize/gradients/score/Dropout/dropout_1/div_grad/SumSum;optimize/gradients/score/Dropout/dropout_1/div_grad/RealDivIoptimize/gradients/score/Dropout/dropout_1/div_grad/BroadcastGradientArgs*
T0*

Tidx0*
	keep_dims( 
Ń
;optimize/gradients/score/Dropout/dropout_1/div_grad/ReshapeReshape7optimize/gradients/score/Dropout/dropout_1/div_grad/Sum9optimize/gradients/score/Dropout/dropout_1/div_grad/Shape*
T0*
Tshape0
Z
7optimize/gradients/score/Dropout/dropout_1/div_grad/NegNegscore/fc1/BiasAdd*
T0

=optimize/gradients/score/Dropout/dropout_1/div_grad/RealDiv_1RealDiv7optimize/gradients/score/Dropout/dropout_1/div_grad/Negscore/Dropout/sub_1*
T0
Ľ
=optimize/gradients/score/Dropout/dropout_1/div_grad/RealDiv_2RealDiv=optimize/gradients/score/Dropout/dropout_1/div_grad/RealDiv_1score/Dropout/sub_1*
T0
Ô
7optimize/gradients/score/Dropout/dropout_1/div_grad/mulMulLoptimize/gradients/score/Dropout/dropout_1/mul_grad/tuple/control_dependency=optimize/gradients/score/Dropout/dropout_1/div_grad/RealDiv_2*
T0
ě
9optimize/gradients/score/Dropout/dropout_1/div_grad/Sum_1Sum7optimize/gradients/score/Dropout/dropout_1/div_grad/mulKoptimize/gradients/score/Dropout/dropout_1/div_grad/BroadcastGradientArgs:1*
T0*

Tidx0*
	keep_dims( 
×
=optimize/gradients/score/Dropout/dropout_1/div_grad/Reshape_1Reshape9optimize/gradients/score/Dropout/dropout_1/div_grad/Sum_1;optimize/gradients/score/Dropout/dropout_1/div_grad/Shape_1*
T0*
Tshape0
Ę
Doptimize/gradients/score/Dropout/dropout_1/div_grad/tuple/group_depsNoOp<^optimize/gradients/score/Dropout/dropout_1/div_grad/Reshape>^optimize/gradients/score/Dropout/dropout_1/div_grad/Reshape_1
ľ
Loptimize/gradients/score/Dropout/dropout_1/div_grad/tuple/control_dependencyIdentity;optimize/gradients/score/Dropout/dropout_1/div_grad/ReshapeE^optimize/gradients/score/Dropout/dropout_1/div_grad/tuple/group_deps*
T0*N
_classD
B@loc:@optimize/gradients/score/Dropout/dropout_1/div_grad/Reshape
ť
Noptimize/gradients/score/Dropout/dropout_1/div_grad/tuple/control_dependency_1Identity=optimize/gradients/score/Dropout/dropout_1/div_grad/Reshape_1E^optimize/gradients/score/Dropout/dropout_1/div_grad/tuple/group_deps*
T0*P
_classF
DBloc:@optimize/gradients/score/Dropout/dropout_1/div_grad/Reshape_1
˛
5optimize/gradients/score/fc1/BiasAdd_grad/BiasAddGradBiasAddGradLoptimize/gradients/score/Dropout/dropout_1/div_grad/tuple/control_dependency*
T0*
data_formatNHWC
É
:optimize/gradients/score/fc1/BiasAdd_grad/tuple/group_depsNoOpM^optimize/gradients/score/Dropout/dropout_1/div_grad/tuple/control_dependency6^optimize/gradients/score/fc1/BiasAdd_grad/BiasAddGrad
˛
Boptimize/gradients/score/fc1/BiasAdd_grad/tuple/control_dependencyIdentityLoptimize/gradients/score/Dropout/dropout_1/div_grad/tuple/control_dependency;^optimize/gradients/score/fc1/BiasAdd_grad/tuple/group_deps*
T0*N
_classD
B@loc:@optimize/gradients/score/Dropout/dropout_1/div_grad/Reshape

Doptimize/gradients/score/fc1/BiasAdd_grad/tuple/control_dependency_1Identity5optimize/gradients/score/fc1/BiasAdd_grad/BiasAddGrad;^optimize/gradients/score/fc1/BiasAdd_grad/tuple/group_deps*
T0*H
_class>
<:loc:@optimize/gradients/score/fc1/BiasAdd_grad/BiasAddGrad
˝
/optimize/gradients/score/fc1/MatMul_grad/MatMulMatMulBoptimize/gradients/score/fc1/BiasAdd_grad/tuple/control_dependencyfc1/kernel/read*
T0*
transpose_a( *
transpose_b(
ˇ
1optimize/gradients/score/fc1/MatMul_grad/MatMul_1MatMulcnn/gmpBoptimize/gradients/score/fc1/BiasAdd_grad/tuple/control_dependency*
transpose_b( *
T0*
transpose_a(
§
9optimize/gradients/score/fc1/MatMul_grad/tuple/group_depsNoOp0^optimize/gradients/score/fc1/MatMul_grad/MatMul2^optimize/gradients/score/fc1/MatMul_grad/MatMul_1

Aoptimize/gradients/score/fc1/MatMul_grad/tuple/control_dependencyIdentity/optimize/gradients/score/fc1/MatMul_grad/MatMul:^optimize/gradients/score/fc1/MatMul_grad/tuple/group_deps*
T0*B
_class8
64loc:@optimize/gradients/score/fc1/MatMul_grad/MatMul

Coptimize/gradients/score/fc1/MatMul_grad/tuple/control_dependency_1Identity1optimize/gradients/score/fc1/MatMul_grad/MatMul_1:^optimize/gradients/score/fc1/MatMul_grad/tuple/group_deps*
T0*D
_class:
86loc:@optimize/gradients/score/fc1/MatMul_grad/MatMul_1
Y
%optimize/gradients/cnn/gmp_grad/ShapeShapecnn/conv/BiasAdd*
T0*
out_type0
N
$optimize/gradients/cnn/gmp_grad/SizeConst*
value	B :*
dtype0
t
#optimize/gradients/cnn/gmp_grad/addAddcnn/gmp/reduction_indices$optimize/gradients/cnn/gmp_grad/Size*
T0

#optimize/gradients/cnn/gmp_grad/modFloorMod#optimize/gradients/cnn/gmp_grad/add$optimize/gradients/cnn/gmp_grad/Size*
T0
U
'optimize/gradients/cnn/gmp_grad/Shape_1Const*
valueB:*
dtype0
U
+optimize/gradients/cnn/gmp_grad/range/startConst*
value	B : *
dtype0
U
+optimize/gradients/cnn/gmp_grad/range/deltaConst*
value	B :*
dtype0
ş
%optimize/gradients/cnn/gmp_grad/rangeRange+optimize/gradients/cnn/gmp_grad/range/start$optimize/gradients/cnn/gmp_grad/Size+optimize/gradients/cnn/gmp_grad/range/delta*

Tidx0
T
*optimize/gradients/cnn/gmp_grad/Fill/valueConst*
value	B :*
dtype0

$optimize/gradients/cnn/gmp_grad/FillFill'optimize/gradients/cnn/gmp_grad/Shape_1*optimize/gradients/cnn/gmp_grad/Fill/value*
T0*

index_type0
é
-optimize/gradients/cnn/gmp_grad/DynamicStitchDynamicStitch%optimize/gradients/cnn/gmp_grad/range#optimize/gradients/cnn/gmp_grad/mod%optimize/gradients/cnn/gmp_grad/Shape$optimize/gradients/cnn/gmp_grad/Fill*
T0*
N

'optimize/gradients/cnn/gmp_grad/ReshapeReshapecnn/gmp-optimize/gradients/cnn/gmp_grad/DynamicStitch*
T0*
Tshape0
˝
)optimize/gradients/cnn/gmp_grad/Reshape_1ReshapeAoptimize/gradients/score/fc1/MatMul_grad/tuple/control_dependency-optimize/gradients/cnn/gmp_grad/DynamicStitch*
T0*
Tshape0
r
%optimize/gradients/cnn/gmp_grad/EqualEqual'optimize/gradients/cnn/gmp_grad/Reshapecnn/conv/BiasAdd*
T0
{
$optimize/gradients/cnn/gmp_grad/CastCast%optimize/gradients/cnn/gmp_grad/Equal*

SrcT0
*
Truncate( *

DstT0

#optimize/gradients/cnn/gmp_grad/SumSum$optimize/gradients/cnn/gmp_grad/Castcnn/gmp/reduction_indices*
T0*

Tidx0*
	keep_dims( 

)optimize/gradients/cnn/gmp_grad/Reshape_2Reshape#optimize/gradients/cnn/gmp_grad/Sum-optimize/gradients/cnn/gmp_grad/DynamicStitch*
T0*
Tshape0

#optimize/gradients/cnn/gmp_grad/divRealDiv$optimize/gradients/cnn/gmp_grad/Cast)optimize/gradients/cnn/gmp_grad/Reshape_2*
T0

#optimize/gradients/cnn/gmp_grad/mulMul#optimize/gradients/cnn/gmp_grad/div)optimize/gradients/cnn/gmp_grad/Reshape_1*
T0

4optimize/gradients/cnn/conv/BiasAdd_grad/BiasAddGradBiasAddGrad#optimize/gradients/cnn/gmp_grad/mul*
T0*
data_formatNHWC

9optimize/gradients/cnn/conv/BiasAdd_grad/tuple/group_depsNoOp5^optimize/gradients/cnn/conv/BiasAdd_grad/BiasAddGrad$^optimize/gradients/cnn/gmp_grad/mul
ď
Aoptimize/gradients/cnn/conv/BiasAdd_grad/tuple/control_dependencyIdentity#optimize/gradients/cnn/gmp_grad/mul:^optimize/gradients/cnn/conv/BiasAdd_grad/tuple/group_deps*
T0*6
_class,
*(loc:@optimize/gradients/cnn/gmp_grad/mul

Coptimize/gradients/cnn/conv/BiasAdd_grad/tuple/control_dependency_1Identity4optimize/gradients/cnn/conv/BiasAdd_grad/BiasAddGrad:^optimize/gradients/cnn/conv/BiasAdd_grad/tuple/group_deps*
T0*G
_class=
;9loc:@optimize/gradients/cnn/conv/BiasAdd_grad/BiasAddGrad
o
5optimize/gradients/cnn/conv/conv1d/Squeeze_grad/ShapeShapecnn/conv/conv1d/Conv2D*
T0*
out_type0
Ó
7optimize/gradients/cnn/conv/conv1d/Squeeze_grad/ReshapeReshapeAoptimize/gradients/cnn/conv/BiasAdd_grad/tuple/control_dependency5optimize/gradients/cnn/conv/conv1d/Squeeze_grad/Shape*
T0*
Tshape0

5optimize/gradients/cnn/conv/conv1d/Conv2D_grad/ShapeNShapeNcnn/conv/conv1d/ExpandDimscnn/conv/conv1d/ExpandDims_1*
T0*
out_type0*
N
ß
Boptimize/gradients/cnn/conv/conv1d/Conv2D_grad/Conv2DBackpropInputConv2DBackpropInput5optimize/gradients/cnn/conv/conv1d/Conv2D_grad/ShapeNcnn/conv/conv1d/ExpandDims_17optimize/gradients/cnn/conv/conv1d/Squeeze_grad/Reshape*
	dilations
*
T0*
data_formatNHWC*
strides
*
use_cudnn_on_gpu(*
paddingVALID
á
Coptimize/gradients/cnn/conv/conv1d/Conv2D_grad/Conv2DBackpropFilterConv2DBackpropFiltercnn/conv/conv1d/ExpandDims7optimize/gradients/cnn/conv/conv1d/Conv2D_grad/ShapeN:17optimize/gradients/cnn/conv/conv1d/Squeeze_grad/Reshape*
	dilations
*
T0*
data_formatNHWC*
strides
*
use_cudnn_on_gpu(*
paddingVALID
Ň
?optimize/gradients/cnn/conv/conv1d/Conv2D_grad/tuple/group_depsNoOpD^optimize/gradients/cnn/conv/conv1d/Conv2D_grad/Conv2DBackpropFilterC^optimize/gradients/cnn/conv/conv1d/Conv2D_grad/Conv2DBackpropInput
š
Goptimize/gradients/cnn/conv/conv1d/Conv2D_grad/tuple/control_dependencyIdentityBoptimize/gradients/cnn/conv/conv1d/Conv2D_grad/Conv2DBackpropInput@^optimize/gradients/cnn/conv/conv1d/Conv2D_grad/tuple/group_deps*
T0*U
_classK
IGloc:@optimize/gradients/cnn/conv/conv1d/Conv2D_grad/Conv2DBackpropInput
˝
Ioptimize/gradients/cnn/conv/conv1d/Conv2D_grad/tuple/control_dependency_1IdentityCoptimize/gradients/cnn/conv/conv1d/Conv2D_grad/Conv2DBackpropFilter@^optimize/gradients/cnn/conv/conv1d/Conv2D_grad/tuple/group_deps*
T0*V
_classL
JHloc:@optimize/gradients/cnn/conv/conv1d/Conv2D_grad/Conv2DBackpropFilter
u
8optimize/gradients/cnn/conv/conv1d/ExpandDims_grad/ShapeShapeembedding_lookup/Identity*
T0*
out_type0
ß
:optimize/gradients/cnn/conv/conv1d/ExpandDims_grad/ReshapeReshapeGoptimize/gradients/cnn/conv/conv1d/Conv2D_grad/tuple/control_dependency8optimize/gradients/cnn/conv/conv1d/ExpandDims_grad/Shape*
T0*
Tshape0
s
:optimize/gradients/cnn/conv/conv1d/ExpandDims_1_grad/ShapeConst*!
valueB"   @      *
dtype0
ĺ
<optimize/gradients/cnn/conv/conv1d/ExpandDims_1_grad/ReshapeReshapeIoptimize/gradients/cnn/conv/conv1d/Conv2D_grad/tuple/control_dependency_1:optimize/gradients/cnn/conv/conv1d/ExpandDims_1_grad/Shape*
T0*
Tshape0

.optimize/gradients/embedding_lookup_grad/ShapeConst"/device:CPU:0*%
valueB	"      @       *
_class
loc:@embedding*
dtype0	
˝
0optimize/gradients/embedding_lookup_grad/ToInt32Cast.optimize/gradients/embedding_lookup_grad/Shape"/device:CPU:0*

SrcT0	*
_class
loc:@embedding*
Truncate( *

DstT0
W
-optimize/gradients/embedding_lookup_grad/SizeSizeinput_x*
T0*
out_type0
a
7optimize/gradients/embedding_lookup_grad/ExpandDims/dimConst*
value	B : *
dtype0
ž
3optimize/gradients/embedding_lookup_grad/ExpandDims
ExpandDims-optimize/gradients/embedding_lookup_grad/Size7optimize/gradients/embedding_lookup_grad/ExpandDims/dim*
T0*

Tdim0
j
<optimize/gradients/embedding_lookup_grad/strided_slice/stackConst*
valueB:*
dtype0
l
>optimize/gradients/embedding_lookup_grad/strided_slice/stack_1Const*
valueB: *
dtype0
l
>optimize/gradients/embedding_lookup_grad/strided_slice/stack_2Const*
valueB:*
dtype0
°
6optimize/gradients/embedding_lookup_grad/strided_sliceStridedSlice0optimize/gradients/embedding_lookup_grad/ToInt32<optimize/gradients/embedding_lookup_grad/strided_slice/stack>optimize/gradients/embedding_lookup_grad/strided_slice/stack_1>optimize/gradients/embedding_lookup_grad/strided_slice/stack_2*
shrink_axis_mask *
ellipsis_mask *

begin_mask *
new_axis_mask *
end_mask*
Index0*
T0
^
4optimize/gradients/embedding_lookup_grad/concat/axisConst*
value	B : *
dtype0
ü
/optimize/gradients/embedding_lookup_grad/concatConcatV23optimize/gradients/embedding_lookup_grad/ExpandDims6optimize/gradients/embedding_lookup_grad/strided_slice4optimize/gradients/embedding_lookup_grad/concat/axis*

Tidx0*
T0*
N
ż
0optimize/gradients/embedding_lookup_grad/ReshapeReshape:optimize/gradients/cnn/conv/conv1d/ExpandDims_grad/Reshape/optimize/gradients/embedding_lookup_grad/concat*
T0*
Tshape0

2optimize/gradients/embedding_lookup_grad/Reshape_1Reshapeinput_x3optimize/gradients/embedding_lookup_grad/ExpandDims*
T0*
Tshape0
m
"optimize/beta1_power/initial_valueConst*
valueB
 *fff?*
_class
loc:@conv/bias*
dtype0
~
optimize/beta1_power
VariableV2*
_class
loc:@conv/bias*
dtype0*
	container *
shape: *
shared_name 
Ż
optimize/beta1_power/AssignAssignoptimize/beta1_power"optimize/beta1_power/initial_value*
T0*
_class
loc:@conv/bias*
validate_shape(*
use_locking(
b
optimize/beta1_power/readIdentityoptimize/beta1_power*
T0*
_class
loc:@conv/bias
m
"optimize/beta2_power/initial_valueConst*
valueB
 *wž?*
_class
loc:@conv/bias*
dtype0
~
optimize/beta2_power
VariableV2*
_class
loc:@conv/bias*
dtype0*
	container *
shape: *
shared_name 
Ż
optimize/beta2_power/AssignAssignoptimize/beta2_power"optimize/beta2_power/initial_value*
use_locking(*
T0*
_class
loc:@conv/bias*
validate_shape(
b
optimize/beta2_power/readIdentityoptimize/beta2_power*
T0*
_class
loc:@conv/bias

0embedding/Adam/Initializer/zeros/shape_as_tensorConst"/device:CPU:0*
valueB"  @   *
_class
loc:@embedding*
dtype0

&embedding/Adam/Initializer/zeros/ConstConst"/device:CPU:0*
valueB
 *    *
_class
loc:@embedding*
dtype0
Ę
 embedding/Adam/Initializer/zerosFill0embedding/Adam/Initializer/zeros/shape_as_tensor&embedding/Adam/Initializer/zeros/Const"/device:CPU:0*
T0*

index_type0*
_class
loc:@embedding

embedding/Adam
VariableV2"/device:CPU:0*
_class
loc:@embedding*
dtype0*
	container *
shape:	'@*
shared_name 
°
embedding/Adam/AssignAssignembedding/Adam embedding/Adam/Initializer/zeros"/device:CPU:0*
use_locking(*
T0*
_class
loc:@embedding*
validate_shape(
e
embedding/Adam/readIdentityembedding/Adam"/device:CPU:0*
T0*
_class
loc:@embedding

2embedding/Adam_1/Initializer/zeros/shape_as_tensorConst"/device:CPU:0*
valueB"  @   *
_class
loc:@embedding*
dtype0

(embedding/Adam_1/Initializer/zeros/ConstConst"/device:CPU:0*
valueB
 *    *
_class
loc:@embedding*
dtype0
Đ
"embedding/Adam_1/Initializer/zerosFill2embedding/Adam_1/Initializer/zeros/shape_as_tensor(embedding/Adam_1/Initializer/zeros/Const"/device:CPU:0*
T0*

index_type0*
_class
loc:@embedding

embedding/Adam_1
VariableV2"/device:CPU:0*
_class
loc:@embedding*
dtype0*
	container *
shape:	'@*
shared_name 
ś
embedding/Adam_1/AssignAssignembedding/Adam_1"embedding/Adam_1/Initializer/zeros"/device:CPU:0*
use_locking(*
T0*
_class
loc:@embedding*
validate_shape(
i
embedding/Adam_1/readIdentityembedding/Adam_1"/device:CPU:0*
T0*
_class
loc:@embedding

2conv/kernel/Adam/Initializer/zeros/shape_as_tensorConst*!
valueB"   @      *
_class
loc:@conv/kernel*
dtype0
u
(conv/kernel/Adam/Initializer/zeros/ConstConst*
valueB
 *    *
_class
loc:@conv/kernel*
dtype0
Ă
"conv/kernel/Adam/Initializer/zerosFill2conv/kernel/Adam/Initializer/zeros/shape_as_tensor(conv/kernel/Adam/Initializer/zeros/Const*
T0*

index_type0*
_class
loc:@conv/kernel

conv/kernel/Adam
VariableV2*
_class
loc:@conv/kernel*
dtype0*
	container *
shape:@*
shared_name 
Š
conv/kernel/Adam/AssignAssignconv/kernel/Adam"conv/kernel/Adam/Initializer/zeros*
T0*
_class
loc:@conv/kernel*
validate_shape(*
use_locking(
\
conv/kernel/Adam/readIdentityconv/kernel/Adam*
T0*
_class
loc:@conv/kernel

4conv/kernel/Adam_1/Initializer/zeros/shape_as_tensorConst*!
valueB"   @      *
_class
loc:@conv/kernel*
dtype0
w
*conv/kernel/Adam_1/Initializer/zeros/ConstConst*
valueB
 *    *
_class
loc:@conv/kernel*
dtype0
É
$conv/kernel/Adam_1/Initializer/zerosFill4conv/kernel/Adam_1/Initializer/zeros/shape_as_tensor*conv/kernel/Adam_1/Initializer/zeros/Const*
T0*

index_type0*
_class
loc:@conv/kernel

conv/kernel/Adam_1
VariableV2*
_class
loc:@conv/kernel*
dtype0*
	container *
shape:@*
shared_name 
Ż
conv/kernel/Adam_1/AssignAssignconv/kernel/Adam_1$conv/kernel/Adam_1/Initializer/zeros*
T0*
_class
loc:@conv/kernel*
validate_shape(*
use_locking(
`
conv/kernel/Adam_1/readIdentityconv/kernel/Adam_1*
T0*
_class
loc:@conv/kernel
p
 conv/bias/Adam/Initializer/zerosConst*
valueB*    *
_class
loc:@conv/bias*
dtype0
}
conv/bias/Adam
VariableV2*
_class
loc:@conv/bias*
dtype0*
	container *
shape:*
shared_name 
Ą
conv/bias/Adam/AssignAssignconv/bias/Adam conv/bias/Adam/Initializer/zeros*
use_locking(*
T0*
_class
loc:@conv/bias*
validate_shape(
V
conv/bias/Adam/readIdentityconv/bias/Adam*
T0*
_class
loc:@conv/bias
r
"conv/bias/Adam_1/Initializer/zerosConst*
valueB*    *
_class
loc:@conv/bias*
dtype0

conv/bias/Adam_1
VariableV2*
_class
loc:@conv/bias*
dtype0*
	container *
shape:*
shared_name 
§
conv/bias/Adam_1/AssignAssignconv/bias/Adam_1"conv/bias/Adam_1/Initializer/zeros*
T0*
_class
loc:@conv/bias*
validate_shape(*
use_locking(
Z
conv/bias/Adam_1/readIdentityconv/bias/Adam_1*
T0*
_class
loc:@conv/bias

1fc1/kernel/Adam/Initializer/zeros/shape_as_tensorConst*
valueB"      *
_class
loc:@fc1/kernel*
dtype0
s
'fc1/kernel/Adam/Initializer/zeros/ConstConst*
valueB
 *    *
_class
loc:@fc1/kernel*
dtype0
ż
!fc1/kernel/Adam/Initializer/zerosFill1fc1/kernel/Adam/Initializer/zeros/shape_as_tensor'fc1/kernel/Adam/Initializer/zeros/Const*
T0*

index_type0*
_class
loc:@fc1/kernel

fc1/kernel/Adam
VariableV2*
_class
loc:@fc1/kernel*
dtype0*
	container *
shape:
*
shared_name 
Ľ
fc1/kernel/Adam/AssignAssignfc1/kernel/Adam!fc1/kernel/Adam/Initializer/zeros*
T0*
_class
loc:@fc1/kernel*
validate_shape(*
use_locking(
Y
fc1/kernel/Adam/readIdentityfc1/kernel/Adam*
T0*
_class
loc:@fc1/kernel

3fc1/kernel/Adam_1/Initializer/zeros/shape_as_tensorConst*
valueB"      *
_class
loc:@fc1/kernel*
dtype0
u
)fc1/kernel/Adam_1/Initializer/zeros/ConstConst*
valueB
 *    *
_class
loc:@fc1/kernel*
dtype0
Ĺ
#fc1/kernel/Adam_1/Initializer/zerosFill3fc1/kernel/Adam_1/Initializer/zeros/shape_as_tensor)fc1/kernel/Adam_1/Initializer/zeros/Const*
T0*

index_type0*
_class
loc:@fc1/kernel

fc1/kernel/Adam_1
VariableV2*
shape:
*
shared_name *
_class
loc:@fc1/kernel*
dtype0*
	container 
Ť
fc1/kernel/Adam_1/AssignAssignfc1/kernel/Adam_1#fc1/kernel/Adam_1/Initializer/zeros*
use_locking(*
T0*
_class
loc:@fc1/kernel*
validate_shape(
]
fc1/kernel/Adam_1/readIdentityfc1/kernel/Adam_1*
T0*
_class
loc:@fc1/kernel
n
fc1/bias/Adam/Initializer/zerosConst*
valueB*    *
_class
loc:@fc1/bias*
dtype0
{
fc1/bias/Adam
VariableV2*
_class
loc:@fc1/bias*
dtype0*
	container *
shape:*
shared_name 

fc1/bias/Adam/AssignAssignfc1/bias/Adamfc1/bias/Adam/Initializer/zeros*
T0*
_class
loc:@fc1/bias*
validate_shape(*
use_locking(
S
fc1/bias/Adam/readIdentityfc1/bias/Adam*
T0*
_class
loc:@fc1/bias
p
!fc1/bias/Adam_1/Initializer/zerosConst*
valueB*    *
_class
loc:@fc1/bias*
dtype0
}
fc1/bias/Adam_1
VariableV2*
shape:*
shared_name *
_class
loc:@fc1/bias*
dtype0*
	container 
Ł
fc1/bias/Adam_1/AssignAssignfc1/bias/Adam_1!fc1/bias/Adam_1/Initializer/zeros*
use_locking(*
T0*
_class
loc:@fc1/bias*
validate_shape(
W
fc1/bias/Adam_1/readIdentityfc1/bias/Adam_1*
T0*
_class
loc:@fc1/bias
v
!fc2/kernel/Adam/Initializer/zerosConst*
valueB	*    *
_class
loc:@fc2/kernel*
dtype0

fc2/kernel/Adam
VariableV2*
shape:	*
shared_name *
_class
loc:@fc2/kernel*
dtype0*
	container 
Ľ
fc2/kernel/Adam/AssignAssignfc2/kernel/Adam!fc2/kernel/Adam/Initializer/zeros*
T0*
_class
loc:@fc2/kernel*
validate_shape(*
use_locking(
Y
fc2/kernel/Adam/readIdentityfc2/kernel/Adam*
T0*
_class
loc:@fc2/kernel
x
#fc2/kernel/Adam_1/Initializer/zerosConst*
valueB	*    *
_class
loc:@fc2/kernel*
dtype0

fc2/kernel/Adam_1
VariableV2*
shape:	*
shared_name *
_class
loc:@fc2/kernel*
dtype0*
	container 
Ť
fc2/kernel/Adam_1/AssignAssignfc2/kernel/Adam_1#fc2/kernel/Adam_1/Initializer/zeros*
T0*
_class
loc:@fc2/kernel*
validate_shape(*
use_locking(
]
fc2/kernel/Adam_1/readIdentityfc2/kernel/Adam_1*
T0*
_class
loc:@fc2/kernel
m
fc2/bias/Adam/Initializer/zerosConst*
valueB*    *
_class
loc:@fc2/bias*
dtype0
z
fc2/bias/Adam
VariableV2*
shape:*
shared_name *
_class
loc:@fc2/bias*
dtype0*
	container 

fc2/bias/Adam/AssignAssignfc2/bias/Adamfc2/bias/Adam/Initializer/zeros*
T0*
_class
loc:@fc2/bias*
validate_shape(*
use_locking(
S
fc2/bias/Adam/readIdentityfc2/bias/Adam*
T0*
_class
loc:@fc2/bias
o
!fc2/bias/Adam_1/Initializer/zerosConst*
valueB*    *
_class
loc:@fc2/bias*
dtype0
|
fc2/bias/Adam_1
VariableV2*
_class
loc:@fc2/bias*
dtype0*
	container *
shape:*
shared_name 
Ł
fc2/bias/Adam_1/AssignAssignfc2/bias/Adam_1!fc2/bias/Adam_1/Initializer/zeros*
T0*
_class
loc:@fc2/bias*
validate_shape(*
use_locking(
W
fc2/bias/Adam_1/readIdentityfc2/bias/Adam_1*
T0*
_class
loc:@fc2/bias
H
optimize/Adam/learning_rateConst*
valueB
 *o:*
dtype0
@
optimize/Adam/beta1Const*
valueB
 *fff?*
dtype0
@
optimize/Adam/beta2Const*
valueB
 *wž?*
dtype0
B
optimize/Adam/epsilonConst*
valueB
 *wĚ+2*
dtype0
¨
%optimize/Adam/update_embedding/UniqueUnique2optimize/gradients/embedding_lookup_grad/Reshape_1"/device:CPU:0*
T0*
out_idx0*
_class
loc:@embedding

$optimize/Adam/update_embedding/ShapeShape%optimize/Adam/update_embedding/Unique"/device:CPU:0*
T0*
out_type0*
_class
loc:@embedding

2optimize/Adam/update_embedding/strided_slice/stackConst"/device:CPU:0*
valueB: *
_class
loc:@embedding*
dtype0

4optimize/Adam/update_embedding/strided_slice/stack_1Const"/device:CPU:0*
valueB:*
_class
loc:@embedding*
dtype0

4optimize/Adam/update_embedding/strided_slice/stack_2Const"/device:CPU:0*
valueB:*
_class
loc:@embedding*
dtype0
Š
,optimize/Adam/update_embedding/strided_sliceStridedSlice$optimize/Adam/update_embedding/Shape2optimize/Adam/update_embedding/strided_slice/stack4optimize/Adam/update_embedding/strided_slice/stack_14optimize/Adam/update_embedding/strided_slice/stack_2"/device:CPU:0*
T0*
Index0*
_class
loc:@embedding*
shrink_axis_mask*

begin_mask *
ellipsis_mask *
new_axis_mask *
end_mask 
Ş
1optimize/Adam/update_embedding/UnsortedSegmentSumUnsortedSegmentSum0optimize/gradients/embedding_lookup_grad/Reshape'optimize/Adam/update_embedding/Unique:1,optimize/Adam/update_embedding/strided_slice"/device:CPU:0*
T0*
_class
loc:@embedding*
Tnumsegments0*
Tindices0
~
$optimize/Adam/update_embedding/sub/xConst"/device:CPU:0*
valueB
 *  ?*
_class
loc:@embedding*
dtype0
 
"optimize/Adam/update_embedding/subSub$optimize/Adam/update_embedding/sub/xoptimize/beta2_power/read"/device:CPU:0*
T0*
_class
loc:@embedding

#optimize/Adam/update_embedding/SqrtSqrt"optimize/Adam/update_embedding/sub"/device:CPU:0*
T0*
_class
loc:@embedding
Ą
"optimize/Adam/update_embedding/mulMuloptimize/Adam/learning_rate#optimize/Adam/update_embedding/Sqrt"/device:CPU:0*
T0*
_class
loc:@embedding

&optimize/Adam/update_embedding/sub_1/xConst"/device:CPU:0*
valueB
 *  ?*
_class
loc:@embedding*
dtype0
¤
$optimize/Adam/update_embedding/sub_1Sub&optimize/Adam/update_embedding/sub_1/xoptimize/beta1_power/read"/device:CPU:0*
T0*
_class
loc:@embedding
ą
&optimize/Adam/update_embedding/truedivRealDiv"optimize/Adam/update_embedding/mul$optimize/Adam/update_embedding/sub_1"/device:CPU:0*
T0*
_class
loc:@embedding

&optimize/Adam/update_embedding/sub_2/xConst"/device:CPU:0*
valueB
 *  ?*
_class
loc:@embedding*
dtype0

$optimize/Adam/update_embedding/sub_2Sub&optimize/Adam/update_embedding/sub_2/xoptimize/Adam/beta1"/device:CPU:0*
T0*
_class
loc:@embedding
ş
$optimize/Adam/update_embedding/mul_1Mul1optimize/Adam/update_embedding/UnsortedSegmentSum$optimize/Adam/update_embedding/sub_2"/device:CPU:0*
T0*
_class
loc:@embedding

$optimize/Adam/update_embedding/mul_2Mulembedding/Adam/readoptimize/Adam/beta1"/device:CPU:0*
T0*
_class
loc:@embedding
Ä
%optimize/Adam/update_embedding/AssignAssignembedding/Adam$optimize/Adam/update_embedding/mul_2"/device:CPU:0*
use_locking( *
T0*
_class
loc:@embedding*
validate_shape(

)optimize/Adam/update_embedding/ScatterAdd
ScatterAddembedding/Adam%optimize/Adam/update_embedding/Unique$optimize/Adam/update_embedding/mul_1&^optimize/Adam/update_embedding/Assign"/device:CPU:0*
T0*
_class
loc:@embedding*
Tindices0*
use_locking( 
Ç
$optimize/Adam/update_embedding/mul_3Mul1optimize/Adam/update_embedding/UnsortedSegmentSum1optimize/Adam/update_embedding/UnsortedSegmentSum"/device:CPU:0*
T0*
_class
loc:@embedding

&optimize/Adam/update_embedding/sub_3/xConst"/device:CPU:0*
valueB
 *  ?*
_class
loc:@embedding*
dtype0

$optimize/Adam/update_embedding/sub_3Sub&optimize/Adam/update_embedding/sub_3/xoptimize/Adam/beta2"/device:CPU:0*
T0*
_class
loc:@embedding
­
$optimize/Adam/update_embedding/mul_4Mul$optimize/Adam/update_embedding/mul_3$optimize/Adam/update_embedding/sub_3"/device:CPU:0*
T0*
_class
loc:@embedding

$optimize/Adam/update_embedding/mul_5Mulembedding/Adam_1/readoptimize/Adam/beta2"/device:CPU:0*
T0*
_class
loc:@embedding
Č
'optimize/Adam/update_embedding/Assign_1Assignembedding/Adam_1$optimize/Adam/update_embedding/mul_5"/device:CPU:0*
use_locking( *
T0*
_class
loc:@embedding*
validate_shape(

+optimize/Adam/update_embedding/ScatterAdd_1
ScatterAddembedding/Adam_1%optimize/Adam/update_embedding/Unique$optimize/Adam/update_embedding/mul_4(^optimize/Adam/update_embedding/Assign_1"/device:CPU:0*
T0*
_class
loc:@embedding*
Tindices0*
use_locking( 

%optimize/Adam/update_embedding/Sqrt_1Sqrt+optimize/Adam/update_embedding/ScatterAdd_1"/device:CPU:0*
T0*
_class
loc:@embedding
´
$optimize/Adam/update_embedding/mul_6Mul&optimize/Adam/update_embedding/truediv)optimize/Adam/update_embedding/ScatterAdd"/device:CPU:0*
T0*
_class
loc:@embedding

"optimize/Adam/update_embedding/addAdd%optimize/Adam/update_embedding/Sqrt_1optimize/Adam/epsilon"/device:CPU:0*
T0*
_class
loc:@embedding
ł
(optimize/Adam/update_embedding/truediv_1RealDiv$optimize/Adam/update_embedding/mul_6"optimize/Adam/update_embedding/add"/device:CPU:0*
T0*
_class
loc:@embedding
ł
(optimize/Adam/update_embedding/AssignSub	AssignSub	embedding(optimize/Adam/update_embedding/truediv_1"/device:CPU:0*
T0*
_class
loc:@embedding*
use_locking( 
ă
)optimize/Adam/update_embedding/group_depsNoOp)^optimize/Adam/update_embedding/AssignSub*^optimize/Adam/update_embedding/ScatterAdd,^optimize/Adam/update_embedding/ScatterAdd_1"/device:CPU:0*
_class
loc:@embedding

*optimize/Adam/update_conv/kernel/ApplyAdam	ApplyAdamconv/kernelconv/kernel/Adamconv/kernel/Adam_1optimize/beta1_power/readoptimize/beta2_power/readoptimize/Adam/learning_rateoptimize/Adam/beta1optimize/Adam/beta2optimize/Adam/epsilon<optimize/gradients/cnn/conv/conv1d/ExpandDims_1_grad/Reshape*
use_locking( *
T0*
_class
loc:@conv/kernel*
use_nesterov( 

(optimize/Adam/update_conv/bias/ApplyAdam	ApplyAdam	conv/biasconv/bias/Adamconv/bias/Adam_1optimize/beta1_power/readoptimize/beta2_power/readoptimize/Adam/learning_rateoptimize/Adam/beta1optimize/Adam/beta2optimize/Adam/epsilonCoptimize/gradients/cnn/conv/BiasAdd_grad/tuple/control_dependency_1*
use_locking( *
T0*
_class
loc:@conv/bias*
use_nesterov( 

)optimize/Adam/update_fc1/kernel/ApplyAdam	ApplyAdam
fc1/kernelfc1/kernel/Adamfc1/kernel/Adam_1optimize/beta1_power/readoptimize/beta2_power/readoptimize/Adam/learning_rateoptimize/Adam/beta1optimize/Adam/beta2optimize/Adam/epsilonCoptimize/gradients/score/fc1/MatMul_grad/tuple/control_dependency_1*
T0*
_class
loc:@fc1/kernel*
use_nesterov( *
use_locking( 

'optimize/Adam/update_fc1/bias/ApplyAdam	ApplyAdamfc1/biasfc1/bias/Adamfc1/bias/Adam_1optimize/beta1_power/readoptimize/beta2_power/readoptimize/Adam/learning_rateoptimize/Adam/beta1optimize/Adam/beta2optimize/Adam/epsilonDoptimize/gradients/score/fc1/BiasAdd_grad/tuple/control_dependency_1*
use_locking( *
T0*
_class
loc:@fc1/bias*
use_nesterov( 

)optimize/Adam/update_fc2/kernel/ApplyAdam	ApplyAdam
fc2/kernelfc2/kernel/Adamfc2/kernel/Adam_1optimize/beta1_power/readoptimize/beta2_power/readoptimize/Adam/learning_rateoptimize/Adam/beta1optimize/Adam/beta2optimize/Adam/epsilonCoptimize/gradients/score/fc2/MatMul_grad/tuple/control_dependency_1*
use_locking( *
T0*
_class
loc:@fc2/kernel*
use_nesterov( 

'optimize/Adam/update_fc2/bias/ApplyAdam	ApplyAdamfc2/biasfc2/bias/Adamfc2/bias/Adam_1optimize/beta1_power/readoptimize/beta2_power/readoptimize/Adam/learning_rateoptimize/Adam/beta1optimize/Adam/beta2optimize/Adam/epsilonDoptimize/gradients/score/fc2/BiasAdd_grad/tuple/control_dependency_1*
use_locking( *
T0*
_class
loc:@fc2/bias*
use_nesterov( 

optimize/Adam/mulMuloptimize/beta1_power/readoptimize/Adam/beta1)^optimize/Adam/update_conv/bias/ApplyAdam+^optimize/Adam/update_conv/kernel/ApplyAdam*^optimize/Adam/update_embedding/group_deps(^optimize/Adam/update_fc1/bias/ApplyAdam*^optimize/Adam/update_fc1/kernel/ApplyAdam(^optimize/Adam/update_fc2/bias/ApplyAdam*^optimize/Adam/update_fc2/kernel/ApplyAdam*
T0*
_class
loc:@conv/bias

optimize/Adam/AssignAssignoptimize/beta1_poweroptimize/Adam/mul*
T0*
_class
loc:@conv/bias*
validate_shape(*
use_locking( 
Ą
optimize/Adam/mul_1Muloptimize/beta2_power/readoptimize/Adam/beta2)^optimize/Adam/update_conv/bias/ApplyAdam+^optimize/Adam/update_conv/kernel/ApplyAdam*^optimize/Adam/update_embedding/group_deps(^optimize/Adam/update_fc1/bias/ApplyAdam*^optimize/Adam/update_fc1/kernel/ApplyAdam(^optimize/Adam/update_fc2/bias/ApplyAdam*^optimize/Adam/update_fc2/kernel/ApplyAdam*
T0*
_class
loc:@conv/bias

optimize/Adam/Assign_1Assignoptimize/beta2_poweroptimize/Adam/mul_1*
T0*
_class
loc:@conv/bias*
validate_shape(*
use_locking( 
Î
optimize/Adam/NoOpNoOp^optimize/Adam/Assign^optimize/Adam/Assign_1)^optimize/Adam/update_conv/bias/ApplyAdam+^optimize/Adam/update_conv/kernel/ApplyAdam(^optimize/Adam/update_fc1/bias/ApplyAdam*^optimize/Adam/update_fc1/kernel/ApplyAdam(^optimize/Adam/update_fc2/bias/ApplyAdam*^optimize/Adam/update_fc2/kernel/ApplyAdam
W
optimize/Adam/NoOp_1NoOp*^optimize/Adam/update_embedding/group_deps"/device:CPU:0
A
optimize/AdamNoOp^optimize/Adam/NoOp^optimize/Adam/NoOp_1
C
accuracy/ArgMax/dimensionConst*
value	B :*
dtype0
e
accuracy/ArgMaxArgMaxinput_yaccuracy/ArgMax/dimension*

Tidx0*
T0*
output_type0	
C
accuracy/EqualEqualaccuracy/ArgMaxscore/y_pred_cls*
T0	
M
accuracy/CastCastaccuracy/Equal*

SrcT0
*
Truncate( *

DstT0
<
accuracy/ConstConst*
valueB: *
dtype0
Z
accuracy/MeanMeanaccuracy/Castaccuracy/Const*
T0*

Tidx0*
	keep_dims( 
6
	loss/tagsConst*
valueB
 Bloss*
dtype0
8
lossScalarSummary	loss/tagsoptimize/Mean*
T0
B
accuracy_1/tagsConst*
valueB B
accuracy_1*
dtype0
D

accuracy_1ScalarSummaryaccuracy_1/tagsaccuracy/Mean*
T0
=
Merge/MergeSummaryMergeSummaryloss
accuracy_1*
N
8

save/ConstConst*
valueB Bmodel*
dtype0
ł
save/SaveV2/tensor_namesConst*
valueřBőB	conv/biasBconv/bias/AdamBconv/bias/Adam_1Bconv/kernelBconv/kernel/AdamBconv/kernel/Adam_1B	embeddingBembedding/AdamBembedding/Adam_1Bfc1/biasBfc1/bias/AdamBfc1/bias/Adam_1B
fc1/kernelBfc1/kernel/AdamBfc1/kernel/Adam_1Bfc2/biasBfc2/bias/AdamBfc2/bias/Adam_1B
fc2/kernelBfc2/kernel/AdamBfc2/kernel/Adam_1Boptimize/beta1_powerBoptimize/beta2_power*
dtype0
u
save/SaveV2/shape_and_slicesConst*A
value8B6B B B B B B B B B B B B B B B B B B B B B B B *
dtype0
í
save/SaveV2SaveV2
save/Constsave/SaveV2/tensor_namessave/SaveV2/shape_and_slices	conv/biasconv/bias/Adamconv/bias/Adam_1conv/kernelconv/kernel/Adamconv/kernel/Adam_1	embeddingembedding/Adamembedding/Adam_1fc1/biasfc1/bias/Adamfc1/bias/Adam_1
fc1/kernelfc1/kernel/Adamfc1/kernel/Adam_1fc2/biasfc2/bias/Adamfc2/bias/Adam_1
fc2/kernelfc2/kernel/Adamfc2/kernel/Adam_1optimize/beta1_poweroptimize/beta2_power*%
dtypes
2
e
save/control_dependencyIdentity
save/Const^save/SaveV2*
T0*
_class
loc:@save/Const
Ĺ
save/RestoreV2/tensor_namesConst"/device:CPU:0*
valueřBőB	conv/biasBconv/bias/AdamBconv/bias/Adam_1Bconv/kernelBconv/kernel/AdamBconv/kernel/Adam_1B	embeddingBembedding/AdamBembedding/Adam_1Bfc1/biasBfc1/bias/AdamBfc1/bias/Adam_1B
fc1/kernelBfc1/kernel/AdamBfc1/kernel/Adam_1Bfc2/biasBfc2/bias/AdamBfc2/bias/Adam_1B
fc2/kernelBfc2/kernel/AdamBfc2/kernel/Adam_1Boptimize/beta1_powerBoptimize/beta2_power*
dtype0

save/RestoreV2/shape_and_slicesConst"/device:CPU:0*A
value8B6B B B B B B B B B B B B B B B B B B B B B B B *
dtype0

save/RestoreV2	RestoreV2
save/Constsave/RestoreV2/tensor_namessave/RestoreV2/shape_and_slices"/device:CPU:0*%
dtypes
2

save/AssignAssign	conv/biassave/RestoreV2*
T0*
_class
loc:@conv/bias*
validate_shape(*
use_locking(

save/Assign_1Assignconv/bias/Adamsave/RestoreV2:1*
T0*
_class
loc:@conv/bias*
validate_shape(*
use_locking(

save/Assign_2Assignconv/bias/Adam_1save/RestoreV2:2*
use_locking(*
T0*
_class
loc:@conv/bias*
validate_shape(

save/Assign_3Assignconv/kernelsave/RestoreV2:3*
T0*
_class
loc:@conv/kernel*
validate_shape(*
use_locking(

save/Assign_4Assignconv/kernel/Adamsave/RestoreV2:4*
use_locking(*
T0*
_class
loc:@conv/kernel*
validate_shape(

save/Assign_5Assignconv/kernel/Adam_1save/RestoreV2:5*
T0*
_class
loc:@conv/kernel*
validate_shape(*
use_locking(

save/Assign_6Assign	embeddingsave/RestoreV2:6"/device:CPU:0*
T0*
_class
loc:@embedding*
validate_shape(*
use_locking(

save/Assign_7Assignembedding/Adamsave/RestoreV2:7"/device:CPU:0*
T0*
_class
loc:@embedding*
validate_shape(*
use_locking(

save/Assign_8Assignembedding/Adam_1save/RestoreV2:8"/device:CPU:0*
use_locking(*
T0*
_class
loc:@embedding*
validate_shape(

save/Assign_9Assignfc1/biassave/RestoreV2:9*
use_locking(*
T0*
_class
loc:@fc1/bias*
validate_shape(

save/Assign_10Assignfc1/bias/Adamsave/RestoreV2:10*
use_locking(*
T0*
_class
loc:@fc1/bias*
validate_shape(

save/Assign_11Assignfc1/bias/Adam_1save/RestoreV2:11*
T0*
_class
loc:@fc1/bias*
validate_shape(*
use_locking(

save/Assign_12Assign
fc1/kernelsave/RestoreV2:12*
T0*
_class
loc:@fc1/kernel*
validate_shape(*
use_locking(

save/Assign_13Assignfc1/kernel/Adamsave/RestoreV2:13*
use_locking(*
T0*
_class
loc:@fc1/kernel*
validate_shape(

save/Assign_14Assignfc1/kernel/Adam_1save/RestoreV2:14*
T0*
_class
loc:@fc1/kernel*
validate_shape(*
use_locking(

save/Assign_15Assignfc2/biassave/RestoreV2:15*
use_locking(*
T0*
_class
loc:@fc2/bias*
validate_shape(

save/Assign_16Assignfc2/bias/Adamsave/RestoreV2:16*
use_locking(*
T0*
_class
loc:@fc2/bias*
validate_shape(

save/Assign_17Assignfc2/bias/Adam_1save/RestoreV2:17*
T0*
_class
loc:@fc2/bias*
validate_shape(*
use_locking(

save/Assign_18Assign
fc2/kernelsave/RestoreV2:18*
T0*
_class
loc:@fc2/kernel*
validate_shape(*
use_locking(

save/Assign_19Assignfc2/kernel/Adamsave/RestoreV2:19*
T0*
_class
loc:@fc2/kernel*
validate_shape(*
use_locking(

save/Assign_20Assignfc2/kernel/Adam_1save/RestoreV2:20*
use_locking(*
T0*
_class
loc:@fc2/kernel*
validate_shape(

save/Assign_21Assignoptimize/beta1_powersave/RestoreV2:21*
T0*
_class
loc:@conv/bias*
validate_shape(*
use_locking(

save/Assign_22Assignoptimize/beta2_powersave/RestoreV2:22*
use_locking(*
T0*
_class
loc:@conv/bias*
validate_shape(
č
save/restore_all/NoOpNoOp^save/Assign^save/Assign_1^save/Assign_10^save/Assign_11^save/Assign_12^save/Assign_13^save/Assign_14^save/Assign_15^save/Assign_16^save/Assign_17^save/Assign_18^save/Assign_19^save/Assign_2^save/Assign_20^save/Assign_21^save/Assign_22^save/Assign_3^save/Assign_4^save/Assign_5^save/Assign_9
^
save/restore_all/NoOp_1NoOp^save/Assign_6^save/Assign_7^save/Assign_8"/device:CPU:0
J
save/restore_allNoOp^save/restore_all/NoOp^save/restore_all/NoOp_1
ń
	init/NoOpNoOp^conv/bias/Adam/Assign^conv/bias/Adam_1/Assign^conv/bias/Assign^conv/kernel/Adam/Assign^conv/kernel/Adam_1/Assign^conv/kernel/Assign^fc1/bias/Adam/Assign^fc1/bias/Adam_1/Assign^fc1/bias/Assign^fc1/kernel/Adam/Assign^fc1/kernel/Adam_1/Assign^fc1/kernel/Assign^fc2/bias/Adam/Assign^fc2/bias/Adam_1/Assign^fc2/bias/Assign^fc2/kernel/Adam/Assign^fc2/kernel/Adam_1/Assign^fc2/kernel/Assign^optimize/beta1_power/Assign^optimize/beta2_power/Assign
g
init/NoOp_1NoOp^embedding/Adam/Assign^embedding/Adam_1/Assign^embedding/Assign"/device:CPU:0
&
initNoOp
^init/NoOp^init/NoOp_1"