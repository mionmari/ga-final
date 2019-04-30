// Main source: https://learnopengl.com/PBR/Lighting
// Normal mapping: http://ogldev.atspace.co.uk/www/tutorial26/tutorial26.html

#version 400
out vec4 finalColor;
in vec2 o_uv;
in vec3 o_normal;
in vec3 o_tangent;
in vec3 o_worldPos;

uniform sampler2D u_albedo;
uniform sampler2D u_normal;
uniform sampler2D u_metallic;
uniform sampler2D u_roughness;
uniform sampler2D u_ao;
uniform bool u_lighting_enabled;
uniform vec3 u_cam_pos;

const vec3 lightPos = vec3(1.5, 1.5, 1);
const vec3 lightColor = vec3(20, 20, 20); 
const float PI = 3.14159265359;
  
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);

vec3 CalcBumpedNormal();

void main()
{	
    vec3 albedo     = texture(u_albedo, o_uv).rgb;
    vec3 normal     = CalcBumpedNormal();//o_normal;
    float metallic  = texture(u_metallic, o_uv).r;
    float roughness = texture(u_roughness, o_uv).r;
    float ao        = texture(u_ao, o_uv).r;

    vec3 N = normalize(o_normal);
    vec3 V = normalize(lightPos - o_worldPos);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
	           
    // reflectance equation
    vec3 Lo = vec3(0.0);
        
    // calculate per-light radiance
    vec3 L = normalize(lightPos - o_worldPos);
    vec3 H = normalize(V + L);
    float distance    = length(lightPos - o_worldPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance     = lightColor * attenuation;        
    
    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = FresnelSchlick(max(dot(H, V), 0.0), F0);       
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  
    
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular     = numerator / max(denominator, 0.001);  
        
    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);                
    Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
  
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = Lo + ambient;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  

	if(u_lighting_enabled)
	{
		finalColor = vec4(color, 1.0);
	}
	else
	{
        finalColor = texture(u_albedo, o_uv);
	}
}  

vec3 CalcBumpedNormal()
{
    vec3 normal = normalize(o_normal);
    vec3 tangent = normalize(o_tangent);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = cross(tangent, normal);
    vec3 bumpMapNormal = texture(u_normal, o_uv).xyz;
    bumpMapNormal = 2.0 * bumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 newNormal;
    mat3 TBN = mat3(tangent, bitangent, normal);
    newNormal = TBN * bumpMapNormal;
    newNormal = normalize(newNormal);
    return newNormal;
}


float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      =  roughness* roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = NdotH2 * (a2 - 1.0) + 1.0;
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}  