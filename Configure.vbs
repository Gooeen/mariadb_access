Option Explicit

Dim FileSystemObject, ProjectPath, MySqlPath, ProjectFileContent, Position, InsertContent

Set FileSystemObject = CreateObject("Scripting.FileSystemObject")
ProjectPath = FileSystemObject.GetFolder(".").Path & "\LibMariaDbConnectivity\LibMariaDbConnectivity.vcxproj"

' 如果脚本文件不在工程目录正确位置则提示并退出
If Not FileSystemObject.FileExists(ProjectPath) Then
	MsgBox "This script is not in project."
	Wscript.Quit()
End If

' 获取MySQL/MariaDB数据库的目录
MySqlPath = GetMySqlPath()

' 获取工程文件内容
ProjectFileContent = GetProjectFileContent()

' 如果已经配置好文件则退出
If InStr(ProjectFileContent, "<IncludePath>") <> 0 Then
	MsgBox "已经配置"
	Wscript.Quit()
End If

InsertContent = "    <IncludePath>" & MySqlPath & "\include;$(IncludePath)</IncludePath>" & VbCrLf
InsertContent = InsertContent & "    <LibraryPath>" & MySqlPath & "\lib;$(LibraryPath)</LibraryPath>" & VbCrLf
InsertContent = InsertContent & "    <TargetName>"

ProjectFileContent = Replace(ProjectFileContent, "    <TargetName>", InsertContent)
Call ReWriteProjectFile(ProjectPath, ProjectFileContent)
MsgBox "配置成功"

' 重写工程文件
Sub ReWriteProjectFile(FilePath, Content)
	Dim AdodbStream
	Set AdodbStream = CreateObject("ADODB.Stream")
	AdodbStream.Type=2
	AdodbStream.Open
	AdodbStream.Charset = "UTF-8"
	AdodbStream.WriteText(Content)
	AdodbStream.SaveToFile FilePath, 2
	AdodbStream.Close()
	Set AdodbStream = Nothing
End Sub

' 获取工程文件内容
Function GetProjectFileContent()
	Dim ProjectContent, AdodbStream
	Set AdodbStream = CreateObject("ADODB.Stream")
	AdodbStream.Type=2
	AdodbStream.Mode = 3
	AdodbStream.Charset = "UTF-8"
	AdodbStream.Open
	AdodbStream.LoadFromFile ProjectPath 
	ProjectContent = AdodbStream.ReadText
	AdodbStream.Close()
	Set AdodbStream = Nothing
	GetProjectFileContent = ProjectContent
End Function

' 获取MySQL/MariaDB数据库的目录
Function GetMySqlPath()

	Dim Shell, MyComputerFolder, MyComputerPath, MysqlFolderItem, MysqlFolderPath
	Set Shell = CreateObject("Shell.Application")

	' 我的电脑路径
	Set MyComputerFolder = Shell.Namespace(&H11&).Self
	MyComputerPath = MyComputerFolder.Path

	Dim MysqlFolder
	Set MysqlFolder = Shell.BrowseForFolder(0, "选择MySQL/MariaDB目录：", 0, MyComputerPath)

	If MysqlFolder Is Nothing Then
		Wscript.Quit()
	End If

	Set MysqlFolderItem = MysqlFolder.Self
	MysqlFolderPath = MysqlFolderItem.Path

	If MysqlFolderPath = "" Then
		Wscript.Quit()
	End If

	' 如果是MySQL/MariaDB数据库的目录
	If IsMySQLFolder(MysqlFolderPath) Then
		GetMySqlPath = MysqlFolderPath
	Else
		MsgBox "不是MySQL/MariaDB目录"
		Wscript.Quit()
	End If

End Function

' 判断是否MySQL/MariaDB数据库的目录
Function IsMySQLFolder(Path)
	IsMySQLFolder = False

	Dim Folder, SubFolders, SubFolder

	If InStr(LCase(Path), "mysql") > 0 Or InStr(LCase(Path), "mariadb") > 0 Then
		Set Folder = FileSystemObject.GetFolder(Path)
		Set SubFolders = Folder.SubFolders
		
		For Each SubFolder In SubFolders

			If LCase(Right(SubFolder.Path, Len("include"))) = "include" Then
				Dim IncludeFolder, SubIncludeFolders, SubIncludeFolder

				Set IncludeFolder = FileSystemObject.GetFolder(SubFolder.Path)
				Set SubIncludeFolders = IncludeFolder.SubFolders
				
				For Each SubIncludeFolder In SubIncludeFolders
					If LCase(Right(SubIncludeFolder.Path, Len("mysql"))) = "mysql" Then
						Dim IncludeMysqlFolder, MysqlFiles, MysqlFile
						
						Set IncludeMysqlFolder = FileSystemObject.GetFolder(SubIncludeFolder)
						Set MysqlFiles = IncludeMysqlFolder.Files

						For Each MysqlFile In MysqlFiles
							If LCase(Right(MysqlFile.Path, Len("mysql.h"))) = "mysql.h" Then
								IsMySQLFolder = True
								Exit For
							End If
						Next
					End If

					If IsMySQLFolder Then
						Exit For
					End If
				Next
			End If

			If IsMySQLFolder Then
				Exit For
			End If
		Next
	End If
End Function
