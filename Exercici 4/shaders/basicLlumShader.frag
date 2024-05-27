#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

uniform vec3 posFocus;
uniform vec3 colFocus;

uniform vec3 posFocusCar1;
uniform vec3 posFocusCar2;

const vec3 llumAmbient = vec3(0.2, 0.2, 0.2);

out vec4 FragColor;


vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
  vec3 normSCO = normalize(fnormal);
  vec3 LSCO = normalize(posFocus - fvertex);

  vec3 Lcar1 = normalize(posFocusCar2 - fvertex);
  vec3 Lcar2 = normalize(posFocusCar1 - fvertex);

  vec3 colorSCO = Difus(normSCO, LSCO, colFocus) + Especular(normSCO, LSCO, fvertex, colFocus);

  vec3 colorCar1 = Difus(normSCO, Lcar1, colFocus) + Especular(normSCO, Lcar1, fvertex, colFocus);
  vec3 colorCar2 = Difus(normSCO, Lcar2, colFocus) + Especular(normSCO, Lcar2, fvertex, colFocus);

  FragColor = vec4(Ambient() + colorSCO + colorCar1 + colorCar2, 1.0);
}
