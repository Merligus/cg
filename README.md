# cg
Trabalho de CG

Victor Hugo Ota
Gustavo de Jesus Merli

1. Implementado em Visual Studo

2. Todas as atividades foram implementadas totalmente

3. Manual
	- Os atalhos são os mesmos implementados para o P2.
	- A opção Delete que remove um objeto de cena junto de seus filhos pode ser feito com o comando 
	Ctrl + Delete.
	- A opção de mudança de pai na janela Hierarchy é feita clicando e arrastando a célula para o
	novo pai.

4. Adições
	- No controlador da câmera foi adicionado a mudança de distância da posição da câmera
	ao ponto de foco da câmera. Ela também muda a rapidez da movimentação da câmera.
	- Opção delete.

5. Explicação matemática dos métodos
	- A1, métodos para cálculo das matrizes de projeção, visão e sua inversa: 

		No método para a matriz de projeção (updateProjectionMatrix), se a opção de projeção ortográfica
	fosse selecionada, a função ortho() com seus devidos parâmetros era chamada apenas calculando
	o "width" utilizando o atributo "\_height" e "\_aspectRatio", e caso a opção de projeção perspectiva
	fosse selecionada, a função perspective() era chamada com seus devidos parâmetros. 

		No método para a matriz de visão e sua inversa (updateViewMatrix), foram utilizadas 4 variáveis
	para calcular as matrizes: 
		- WorldUp que é o vetor que aponta para cima e não precisa ser calculado já que é uma constante 
		do tipo vetor (0, 1, 0);
		- DOP (Direction Of Projection) que aponta para onde a câmera está olhando onde seu valor é a negação da terceira coluna
		da matriz 3x3 de rotação onde essa matriz é calculada a partir da conversão do atributo quatérnio
		"rotation" para uma matriz 3x3;
		- Right que aponta para a direita da câmera é calculada fazendo o produto vetorial DOP x WorldUp;
		- VUP (View Up) que aponta para cima da câmera e é calculada fazendo o produto vetorial Right x DOP.

		Além do atributo "position" as variáveis DOP e VUP foram passadas como parâmetro na função lookat()
	que calcula a matriz de visão. A sua inversa foi calculada usando a função inverse().
	- A2, setPosition(p), setEulerAngles(a), setRotation(q):

		No método setPosition(p), o versor DOP foi calculada a partir dos atributos "\_focalPoint",
	ântigo "\_position" e "\_distance" (DOP = (\_focalPoint - \_position)/\_distance), em seguida a
	posição era atualizada com "p" e depois o novo ponto focal era calculado fazendo
	\_focalPoint = \_distance * DOP + \_position. 

		No método setEulerAngles(a), o vetor de ângulos "a" era atribuido a "\_eulerAngles", e 
	em seguida era tratado fazendo módulo 360 para que não ficasse um ângulo muito grande.
	O ângulo y que rotaciona em torno do eixo x era também tratado para que não passasse de 
	89° ou -89° assim ele não invertia a imagem da visão. Caso o DOP fosse calculado a partir 
	do ponto focal e posição da câmera, a configuração dos ângulos de euler não fariam efeito,
	logo para o cálculo de DOP, os ângulos de euler no atributo "\_eulerAngles" eram convertidos 
	para quaterniões e setados ao atributo "\_rotation" do tipo quatérnio, assim DOP era calculado 
	usando a matriz 3x3 da conversão de quatérnio para matriz 3x3 do atributo "\_rotation". O
	ponto focal era recalculado fazendo \_focalPoint = \_distance * DOP + \_position.

		No método setRotation(q), o quatérnio "q" era passado para o atributo de classe "_rotation" apenas.
	- A3, setViewAngle(value), setHeight(value), setAspectRatio(value), setClippingPlanes(F, B), setProjectionType(value), setDistance(value):

		Os métodos setViewAngle(value) setHeight(value), setAspectRatio(value), setClippingPlanes(F, B) 
	e setProjectionType(value) apenas passavam os parâmetros para os respectivos atributos de classe.

		O método setDistance(value) calculava DOP a partir do ponto focal e posição da câmera e 
	calculava o novo ponto focal que seria mais distante além de passar o parâmetro "value" 
	ao atributo "\_distance".
	- A4, zoom(s), translate(dx, dy, dz), rotateYX(ay, ax, orbit):

		O método zoom(s), caso o modo de projeção fosse paralelo, dividia o atributo "\_height" pelo
	parâmetro "zoom" e caso o modo de projeção fosse perspectiva dividia o atributo "\_viewAngle" 
	por "zoom".

		O método translate(dx, dy, dz) usa as variáveis WorldUp, Right e VUP para calcular o ponto
	focal e a posição da câmera. O WorldUp aponta sempre no sentido positivo do eixo y e é usado para 
	calcular a variável Right que aponta para a direita da câmera (Right = DOP x WorldUp) e a variável 
	VUP que aponta no sentido cima da câmera (VUP = Right x DOP). Com as variáveis DOP e dz é possível calcular 
	a intensidade da movimentação no plano xz no sentido frente e trás da câmera. Com as variáveis Right
	e dx é possível calcular a intensidade da movimentação no plano xz no sentido esquerda e direita da 
	câmera. Com as variáveis VUP e dy é possível calcular a intensidade da movimentação no plano xy no 
	sentido cima e baixo da câmera.

		O método rotateYX(ay, ax, orbit), ay graus rotaciona no eixo y logo é somado ao atributo 
	"\_eulerAngles.x" e e ax graus rotaciona no eixo x logo é somado ao atributo "\_eulerAngles.y". 
	As variáveis x e y de "\_eulerAngles" são tratadas para não passar de +/- 360° e +/-89° respectivamente. 
	Os ângulos de euler no atributo "\_eulerAngles" foram convertidos para quaterniões e setados ao 
	atributo "\_rotation" do tipo quatérnio e então convertido para matriz 3x3, assim DOP era igual 
	a negação da terceira coluna da matriz 3x3 rotation. Se o parâmetro "orbit" fosse verdadeiro, 
	apenas a posição era recalculada, caso contrário o ponto focal era recalculado.

6. Objetos de cena
	- grass (Gras_0#.obj) -> https://free3d.com/3d-model/grass-pack-71861.html
	- resident building (Residential Buildings 0##.obj) -> https://free3d.com/3d-model/array-house-example-3033.html
	- male base (Human.obj) -> https://free3d.com/3d-model/male-base-mesh-6682.html
	- iron man (IronMan.obj) -> https://free3d.com/3d-model/ironman-rigged-original-model--98611.html
	- house (House15.obj) -> https://free3d.com/3d-model/house-15-71141.html
	- bmw (BMW X5 4.obj) -> https://free3d.com/3d-model/bmw-x5-1542.html
	- lamborghini (Avent.obj) -> https://free3d.com/3d-model/lamborghini-aventador-42591.html
	- tree (Tree.obj) -> https://free3d.com/3d-model/tree-74556.html
	- knight (knight.obj) -> https://free3d.com/3d-model/knight-13720.html
	- torre eiffel (Torre_Eiffel.obj) -> https://www.yobi3d.com/c/esViilOvIu1P0Blk