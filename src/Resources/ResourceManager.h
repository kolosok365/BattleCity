#pragma once
#include<string>
#include<memory>//чтобы использовать интеллектуальные указатели.
#include<map>

namespace Renderer
{
	class ShaderProgram;
}

class ResourceManager
{
public:
	ResourceManager(const std::string& executablePath);
	~ResourceManager() = default;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	std::shared_ptr<Renderer::ShaderProgram> loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath); //Шаблон класса std::shared_ptr определяет общий указатель, который может делиться правами собственности на объект с другими общими указателями. 
																			//Поведение совместного доступа реализуется с помощью метода, известного как подсчет ссылок, при котором количество общих указателей, указывающих на объект, 
																			  //сохраняется вместе с ним. Когда это число достигает нуля, либо путем уничтожения или переназначения последнего экземпляра std::shared_ptr , объект автоматически уничтожается.
	std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string& shaderName);

private:
	std::string getFileString(const std::string& relativeFilePath) const;
	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramMap;//состоит из названия шейдера и указателя на шейдерную программу
	ShaderProgramMap m_shaderPrograms;
	std::string m_path;

};