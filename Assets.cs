namespace metroidvania;

public static class Assets
{
    public static AssetTypeManager<Tileset> Tilesets { get; } = new(Tileset.Load);
    
    public class AssetTypeManager<T>(Func<string, T> loader)
    {
        public Func<string, T> Load { get; } = loader;

        private Dictionary<string, T> Assets { get; } = new();

        public T Get(string id) => !this.Assets.TryGetValue(id, out T? asset)
                                       ? throw new Exception($"Asset of type '{typeof(T)}' not found: '{id}'")
                                       : asset;

        public void LoadAll(string filepath, string fileExtension)
        {
            string[] files = Directory.GetFiles(filepath, fileExtension);
            if (files.Length == 0)
                return;

            foreach (string file in files)
            {
                T? asset = this.Load(file) ??
                           throw new Exception($"Asset of type '{typeof(T)}' failed to load: '{file}'");
                this.Assets.Add(file[filepath.Length..^(fileExtension.Length - 1)], asset);
            }
        }

        public void LoadAll(string[] filepaths, string fileExtension)
        {
            foreach (string filepath in filepaths)
                this.LoadAll(filepath, fileExtension);
        }

        public void LoadAll(string filepath, string[] fileExtensions)
        {
            foreach (string fileExtension in fileExtensions)
                this.LoadAll(filepath, fileExtension);
        }

        public void LoadAll(string[] filepaths, string[] fileExtensions)
        {
            foreach (string filepath in filepaths)
            foreach (string fileExtension in fileExtensions)
                this.LoadAll(filepath, fileExtension);
        }
    }
}
