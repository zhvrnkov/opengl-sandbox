#include <glm/glm.hpp>
using namespace glm;

typedef struct {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
} Material;

Material emerald = {
  .ambient = vec3( 0.0215, 0.1745, 0.0215),
  .diffuse = vec3( 0.07568, 0.61424, 0.07568),
  .specular = vec3( 0.633, 0.727811, 0.633),
  .shininess = 0.6
};
Material jade = {
  .ambient = vec3( 0.135, 0.2225, 0.1575),
  .diffuse = vec3( 0.54, 0.89, 0.63),
  .specular = vec3( 0.316228, 0.316228, 0.316228),
  .shininess = 0.1
};
Material obsidian = {
  .ambient = vec3( 0.05375, 0.05, 0.06625),
  .diffuse = vec3( 0.18275, 0.17, 0.22525),
  .specular = vec3( 0.332741, 0.328634, 0.346435),
  .shininess = 0.3
};
Material pearl = {
  .ambient = vec3( 0.25, 0.20725, 0.20725),
  .diffuse = vec3( 1.0, 0.829, 0.829),
  .specular = vec3( 0.296648, 0.296648, 0.296648),
  .shininess = 0.088
};
Material ruby = {
  .ambient = vec3( 0.1745, 0.01175, 0.01175),
  .diffuse = vec3( 0.61424, 0.04136, 0.04136),
  .specular = vec3( 0.727811, 0.626959, 0.626959),
  .shininess = 0.6
};
Material turquoise = {
  .ambient = vec3( 0.1, 0.18725, 0.1745),
  .diffuse = vec3( 0.396, 0.74151, 0.69102),
  .specular = vec3( 0.297254, 0.30829, 0.306678),
  .shininess = 0.1
};
Material brass = {
  .ambient = vec3( 0.329412, 0.223529, 0.027451),
  .diffuse = vec3( 0.780392, 0.568627, 0.113725),
  .specular = vec3( 0.992157, 0.941176, 0.807843),
  .shininess = 0.21794872
};
Material bronze = {
  .ambient = vec3( 0.2125, 0.1275, 0.054),
  .diffuse = vec3( 0.714, 0.4284, 0.18144),
  .specular = vec3( 0.393548, 0.271906, 0.166721),
  .shininess = 0.2
};
Material chrome = {
  .ambient = vec3( 0.25, 0.25, 0.25),
  .diffuse = vec3( 0.4, 0.4, 0.4),
  .specular = vec3( 0.774597, 0.774597, 0.774597),
  .shininess = 0.6
};
Material copper = {
  .ambient = vec3( 0.19125, 0.0735, 0.0225),
  .diffuse = vec3( 0.7038, 0.27048, 0.0828),
  .specular = vec3( 0.256777, 0.137622, 0.086014),
  .shininess = 0.1
};
Material gold = {
  .ambient = vec3( 0.24725, 0.1995, 0.0745),
  .diffuse = vec3( 0.75164, 0.60648, 0.22648),
  .specular = vec3( 0.628281, 0.555802, 0.366065),
  .shininess = 0.4
};
Material silver = {
  .ambient = vec3( 0.19225, 0.19225, 0.19225),
  .diffuse = vec3( 0.50754, 0.50754, 0.50754),
  .specular = vec3( 0.508273, 0.508273, 0.508273),
  .shininess = 0.4
};
Material blackplastic = {
  .ambient = vec3( 0.0, 0.0, 0.0),
  .diffuse = vec3( 0.01, 0.01, 0.01),
  .specular = vec3( 0.50, 0.50, 0.50),
  .shininess = 0.25
};
Material cyanplastic = {
  .ambient = vec3( 0.0, 0.1, 0.06),
  .diffuse = vec3( 0.0, 0.50980392, 0.50980392),
  .specular = vec3( 0.50196078, 0.50196078, 0.50196078),
  .shininess = 0.25
};
Material greenplastic = {
  .ambient = vec3( 0.0, 0.0, 0.0),
  .diffuse = vec3( 0.1, 0.35, 0.1),
  .specular = vec3( 0.45, 0.55, 0.45),
  .shininess = 0.25
};
Material redplastic = {
  .ambient = vec3( 0.0, 0.0, 0.0),
  .diffuse = vec3( 0.5, 0.0, 0.0),
  .specular = vec3( 0.7, 0.6, 0.6),
  .shininess = 0.25
};
Material whiteplastic = {
  .ambient = vec3( 0.0, 0.0, 0.0),
  .diffuse = vec3( 0.55, 0.55, 0.55),
  .specular = vec3( 0.70, 0.70, 0.70),
  .shininess = 0.25
};
Material yellowplastic = {
  .ambient = vec3( 0.0, 0.0, 0.0),
  .diffuse = vec3( 0.5, 0.5, 0.0),
  .specular = vec3( 0.60, 0.60, 0.50),
  .shininess = 0.25
};
Material blackrubber = {
  .ambient = vec3( 0.02, 0.02, 0.02),
  .diffuse = vec3( 0.01, 0.01, 0.01),
  .specular = vec3( 0.4, 0.4, 0.4),
  .shininess = 0.078125
};
Material cyanrubber = {
  .ambient = vec3( 0.0, 0.05, 0.05),
  .diffuse = vec3( 0.4, 0.5, 0.5),
  .specular = vec3( 0.04, 0.7, 0.7),
  .shininess = 0.078125
};
Material greenrubber = {
  .ambient = vec3( 0.0, 0.05, 0.0),
  .diffuse = vec3( 0.4, 0.5, 0.4),
  .specular = vec3( 0.04, 0.7, 0.04),
  .shininess = 0.078125
};
Material redrubber = {
  .ambient = vec3( 0.05, 0.0, 0.0),
  .diffuse = vec3( 0.5, 0.4, 0.4),
  .specular = vec3( 0.7, 0.04, 0.04),
  .shininess = 0.078125
};
Material whiterubber = {
  .ambient = vec3( 0.05, 0.05, 0.05),
  .diffuse = vec3( 0.5, 0.5, 0.5),
  .specular = vec3( 0.7, 0.7, 0.7),
  .shininess = 0.078125
};
Material yellowrubber = {
  .ambient = vec3( 0.05, 0.05, 0.0),
  .diffuse = vec3( 0.5, 0.5, 0.4),
  .specular = vec3( 0.7, 0.7, 0.04),
  .shininess = 0.078125
};

Material materials[] = {
  emerald,
  jade,
  obsidian,
  pearl,
  ruby,
  turquoise,
  brass,
  bronze,
  chrome,
  copper,
  gold,
  silver,
  blackplastic,
  cyanplastic,
  greenplastic,
  redplastic,
  whiteplastic,
  yellowplastic,
  blackrubber,
  cyanrubber,
  greenrubber,
  redrubber,
  whiterubber,
  yellowrubber
};
